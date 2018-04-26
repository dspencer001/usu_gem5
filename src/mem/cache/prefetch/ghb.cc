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

    insertEntry(pc, blk_addr, master_id);

    std::pair<int, long long> stride_info = getStride(pc, master_id);
    

    DPRINTF(HWPrefetch, "hit: PC %x blk_addr %x stride %d, conf %d\n",
                pc, blk_addr, stride_info.second, stride_info.first);

    if(stride_info.first >= 2)
    {
        for (int d = 1; d <= degree; d++) {
            Addr new_addr = blk_addr + d * stride_info.second;
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
}

void
GHBPrefetcher::insertEntry(const Addr pc, Addr blk_addr, MasterID master_id) {
    std::map<Addr, std::vector<StrideEntry>::iterator> &index_table = index_tables[master_id];
    std::vector<StrideEntry> &ghb = ghbs[master_id];
    std::vector<StrideEntry>::iterator &head = heads[master_id];
    std::map<Addr, std::vector<StrideEntry>::iterator>::iterator result;

    if(head->miss_pc > 0) {
        result = index_table.find(head->miss_pc);

        assert(result != index_table.end());

        if(head->next != ghb.end()) {
            index_table.erase(result);
            std::pair<Addr, std::vector<StrideEntry>::iterator> new_val(head->miss_pc, head->next);
            index_table.insert(new_val);
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
        std::vector<StrideEntry>::iterator last_entry = addressListEnd(result->second, master_id);

        if(last_entry != head)
        {
            last_entry->next = head;
            head->prev_miss_addr = last_entry->miss_addr;
        }

    } else {
        // index miss
        index_table.insert(std::make_pair(pc, head));
    }

    head++;

    if (head == ghb.end()) {
        head = ghb.begin();
    }
}

std::vector<GHBPrefetcher::StrideEntry>::iterator GHBPrefetcher::addressListEnd(std::vector<StrideEntry>::iterator it, MasterID master_id) {
    const std::vector<StrideEntry> &ghb = ghbs[master_id];

    while(it->next != ghb.end()) {
        it = it->next;
    }

    return it;
}

std::pair<int, Addr> GHBPrefetcher::getStride(const Addr pc, MasterID master_id) {
    std::map<Addr, std::vector<StrideEntry>::iterator> &index_table = index_tables[master_id];
    std::vector<StrideEntry> &ghb = ghbs[master_id];
    std::map<Addr, std::vector<StrideEntry>::iterator>::iterator result;

    int confidence = 0;
    long long stride = 0;
    Addr previous_stride = 0;

    result = index_table.find(pc);

    assert(result != index_table.end());

    std::vector<StrideEntry>::iterator it = result->second;

    while(it != ghb.end()) {
        if(it->prev_miss_addr > 0) {
            if(it->miss_addr > it->prev_miss_addr)
            {
                stride = it->miss_addr - it->prev_miss_addr;
            } else {
                stride = (it->prev_miss_addr - it->miss_addr) * (-1);
            }
        }

        //DPRINTF(HWPrefetch, "miss_addr: %x, prev_miss_addr: %x\n", it->miss_addr, it->prev_miss_addr);

        if(stride != 0 && (stride == previous_stride)) {
            confidence++;
        } else {
            confidence = 0;
        }

        previous_stride = stride;
        it = it->next;
    }

    return std::make_pair(confidence, stride);
}


GHBPrefetcher*
GHBPrefetcherParams::create()
{
    return new GHBPrefetcher(this);
}
