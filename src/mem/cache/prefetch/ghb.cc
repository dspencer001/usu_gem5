/*
 * Copyright (c) 2005 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Ron Dreslinski
 *          Steve Reinhardt
 */

/**
 * @file
 * GHB Prefetcher implementation.
 */

#include "base/trace.hh"
#include "debug/HWPrefetch.hh"
#include "mem/cache/prefetch/ghb.hh"

void
GHBPrefetcher::calculatePrefetch(PacketPtr &pkt, std::list<Addr> &addresses,
        std::list<Cycles> &delays)
{
    if (!pkt->req->hasPC()) {
        DPRINTF(HWPrefetch, "ignoring request with no PC");
        return;
    }

    Addr blk_addr = pkt->getAddr() & ~(Addr)(blkSize-1);
    int master_id = useMasterId ? pkt->req->masterId() : 0;
    assert(master_id < Max_Masters);
    Addr pc = pkt->req->getPC();
    std::vector<DeltaEntry> &ghb = ghbs[master_id];

    std::vector<DeltaEntry>::iterator entry = insertEntry(pc, blk_addr, master_id);

    std::vector<DeltaEntry>::iterator match_iterator = deltaCorrelation(entry, master_id);
    
    if(match_iterator == ghb.end()) {
        DPRINTF(HWPrefetch, "miss: Master %d PC %x blk_addr %x prev_blk_addr %x delta %d\n", master_id, pc, blk_addr, entry->prev_miss_addr, (long long)(entry->miss_addr) - (long long)(entry->prev_miss_addr));
        return;
    }

    DPRINTF(HWPrefetch, "hit: Master %d PC %x blk_addr %x delta %d\n", master_id, pc, blk_addr, (long long)(entry->miss_addr) - (long long)(entry->prev_miss_addr));

    long long delta = 0;
    match_iterator++;

    for(int d = 0; d < degree; d++) {
        if(match_iterator->prev_miss_addr == 0) {
            break;
        }

        delta += match_iterator->miss_addr - match_iterator->prev_miss_addr;
        Addr new_addr = blk_addr + delta;

        if (pageStop && !samePage(blk_addr, new_addr)) {
            // Spanned the page, so now stop
            pfSpanPage += degree - d + 1;
            return;
        } else {
            DPRINTF(HWPrefetch, "  queuing prefetch to %x @ %d\n",
                    new_addr, latency);
            addresses.push_back(new_addr);
            delays.push_back(latency);
        }
    }
}

std::vector<GHBPrefetcher::DeltaEntry>::iterator
GHBPrefetcher::insertEntry(const Addr pc, Addr blk_addr, MasterID master_id) {
    std::map<long long, std::vector<DeltaEntry>::iterator> &index_table = index_tables[master_id];
    std::vector<DeltaEntry> &ghb = ghbs[master_id];
    std::vector<DeltaEntry>::iterator &head = heads[master_id];
    std::vector<DeltaEntry>::iterator prev_head = heads[master_id];
    std::vector<DeltaEntry>::iterator previous_it;
    std::map<long long, std::vector<DeltaEntry>::iterator>::iterator result;

    if(head->miss_pc > 0) {
        long long head_delta = head->miss_addr - head->prev_miss_addr;
        result = index_table.find(head_delta);

        assert(result != index_table.end());

        if(head->next != ghb.end()) {
            index_table.erase(result);
            index_table.insert(std::make_pair(head_delta, head->next));
        } else {
            index_table.erase(result);
        }
    }

    if(head == ghb.begin()) {
        previous_it = ghb.end() - 1;
    } else {
        previous_it = head - 1;
    }


    head->miss_pc = pc;
    head->miss_addr = blk_addr;
    head->next = ghb.end();
    head->prev_miss_addr = 0;

    if(previous_it->miss_addr > 0) {
        head->prev_miss_addr = previous_it->miss_addr;
    }

    long long head_delta = head->miss_addr - head->prev_miss_addr;

    result = index_table.find(head_delta);

    // index hit
    if(result != index_table.end()) {
        std::vector<DeltaEntry>::iterator last_entry = addressListEnd(result->second, master_id);

        if(last_entry != head)
        {
            last_entry->next = head;
        }
    } else {
        // index miss
        index_table.insert(std::make_pair(head_delta, head));
    }

    head++;

    if (head == ghb.end()) {
        head = ghb.begin();
    }

    return prev_head;
}

std::vector<GHBPrefetcher::DeltaEntry>::iterator GHBPrefetcher::addressListEnd(std::vector<DeltaEntry>::iterator it, MasterID master_id) {
    const std::vector<DeltaEntry> &ghb = ghbs[master_id];

    while(it->next != ghb.end()) {
        it = it->next;
    }

    return it;
}

std::vector<GHBPrefetcher::DeltaEntry>::iterator GHBPrefetcher::deltaCorrelation(
    std::vector<DeltaEntry>::iterator entry,
    MasterID master_id)
{
    std::map<long long, std::vector<DeltaEntry>::iterator> &index_table = index_tables[master_id];
    std::vector<DeltaEntry> &ghb = ghbs[master_id];
    std::map<long long, std::vector<DeltaEntry>::iterator>::iterator result;
    std::vector<DeltaEntry>::iterator correlation_it = ghb.end();

    long long entry_delta = entry->miss_addr - entry->prev_miss_addr;
    std::pair<bool, long long> entry_prev_result = getPreviousDelta(entry, master_id);

    if(!entry_prev_result.first) {
//        DPRINTF(
//            HWPrefetch,
//            "Didn't find a previous address: PC %x blk_addr %x\n",
//            entry->miss_pc,
//            entry->miss_addr
//        );
        return correlation_it;
    }

    result = index_table.find(entry_delta);

    assert(result != index_table.end());

    std::vector<DeltaEntry>::iterator it = result->second;

//    DPRINTF(
//        HWPrefetch,
//        "Looking for match of (%d, %d)\n",
//        entry_delta,
//        entry_prev_result.second
//    );

    while(it != ghb.end() && it != entry) {
        std::pair<bool, long long> prev_delta_result = getPreviousDelta(it, master_id);
//        DPRINTF(
//            HWPrefetch,
//            "Possible match: (%d, %d)\n",
//            entry_delta,
//            prev_delta_result.second
//        );


        if(prev_delta_result.first && prev_delta_result.second == entry_prev_result.second) {
            correlation_it = it;
        }
        
        it = it->next;
    }

    return correlation_it;
}

std::pair<bool, long long>
GHBPrefetcher::getPreviousDelta(std::vector<DeltaEntry>::iterator it, MasterID master_id) {
  std::vector<DeltaEntry> &ghb = ghbs[master_id];
  std::vector<DeltaEntry>::iterator previous_it;

  if(it->prev_miss_addr > 0) {
    if(it == ghb.begin()) {
      previous_it = ghb.end() - 1;
    } else {
      previous_it = it - 1;
    }

    return std::make_pair(true, previous_it->miss_addr - previous_it->prev_miss_addr);
  }

  return std::make_pair(false, 0);
}

GHBPrefetcher*
GHBPrefetcherParams::create()
{
    return new GHBPrefetcher(this);
}
