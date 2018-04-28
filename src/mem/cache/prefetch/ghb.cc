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

    std::vector<long long> delta_chain = deltaCorrelation(entry, master_id);
    
    if(delta_chain.empty()) {
        DPRINTF(HWPrefetch, "miss: Master %d PC %x blk_addr %x prev_blk_addr %x delta %d ghb index %d\n", master_id, pc, blk_addr, entry->prev_miss_addr, (long long)(entry->miss_addr) - (long long)(entry->prev_miss_addr), entry - ghb.begin());
        return;
    }

    DPRINTF(HWPrefetch, "hit: Master %d PC %x blk_addr %x delta %d ghb index %d \n", master_id, pc, blk_addr, (long long)(entry->miss_addr) - (long long)(entry->prev_miss_addr), entry - ghb.begin());

    long long delta = 0;

    std::vector<long long>::iterator it = delta_chain.begin();
    for(int d = 0; d < degree && (it != delta_chain.end()); d++, it++) {
        delta += *it;
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
    std::map<Addr, std::vector<DeltaEntry>::iterator> &index_table = index_tables[master_id];
    std::vector<DeltaEntry> &ghb = ghbs[master_id];
    std::vector<DeltaEntry>::iterator &head = heads[master_id];
    std::vector<DeltaEntry>::iterator prev_head = heads[master_id];
    std::map<Addr, std::vector<DeltaEntry>::iterator>::iterator result;

    if(head->miss_pc > 0) {
        result = index_table.find(head->miss_pc);

        assert(result != index_table.end());

        if(result->second != head) {
            std::vector<DeltaEntry>::iterator prev_entry = addressListEnd(result->second, head);
            prev_entry->next = ghb.end();
        } else {
            index_table.erase(result);
        }
    }

    head->miss_pc = pc;
    head->miss_addr = blk_addr;
    head->next = ghb.end();
    head->prev_miss_addr = 0;

    result = index_table.find(head->miss_pc);

    // index hit
    if(result != index_table.end()) {
        head->next = result->second;
        head->prev_miss_addr = result->second->miss_addr;
        index_table.erase(result);
    }

    index_table.insert(std::make_pair(head->miss_pc, head));

    head++;

    if (head == ghb.end()) {
        head = ghb.begin();
    }

    return prev_head;
}

std::vector<GHBPrefetcher::DeltaEntry>::iterator GHBPrefetcher::addressListEnd(std::vector<DeltaEntry>::iterator it, std::vector<DeltaEntry>::iterator end_it) {
    while(it->next != end_it) {
        it = it->next;
    }

    return it;
}

std::vector<long long> GHBPrefetcher::deltaCorrelation(
    std::vector<DeltaEntry>::iterator entry,
    MasterID master_id)
{
    std::vector<DeltaEntry> &ghb = ghbs[master_id];
    std::map<Addr, std::vector<DeltaEntry>::iterator>::iterator result;
    std::vector<long long> delta_chain;
    std::vector<DeltaEntry>::iterator prev_entry = entry->next;

    if(
        prev_entry == ghb.end() ||
        prev_entry->next == ghb.end() ||
        prev_entry->next->next == ghb.end()
    ) {
        return delta_chain;
    }

    long long delta_1 = entry->miss_addr - entry->next->miss_addr;
    long long delta_2 = prev_entry->miss_addr - prev_entry->next->miss_addr;

    bool matched = false;

    do {
        entry = prev_entry;
        prev_entry = prev_entry->next;

        long long entry_delta = entry->miss_addr - entry->next->miss_addr;
        long long prev_delta = prev_entry->miss_addr - prev_entry->next->miss_addr;

        if(entry_delta == delta_1 && prev_delta == delta_2) {
            matched = true;
        } else {
            delta_chain.push_back(entry_delta);
        }
        
    } while(!matched && (prev_entry->next != ghb.end()));

    std::reverse(delta_chain.begin(), delta_chain.end());

    if(matched) {
        delta_chain.push_back(delta_2);
        delta_chain.push_back(delta_1);
    }

    return delta_chain;
}

GHBPrefetcher*
GHBPrefetcherParams::create()
{
    return new GHBPrefetcher(this);
}
