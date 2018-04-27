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
 */

/**
 * @file
 * Describes a ghb prefetcher.
 */

#ifndef __MEM_CACHE_PREFETCH_GHB_PREFETCHER_HH__
#define __MEM_CACHE_PREFETCH_GHB_PREFETCHER_HH__

#include "mem/cache/prefetch/base.hh"
#include "params/GHBPrefetcher.hh"

class GHBPrefetcher : public BasePrefetcher
{
  public:
    struct DeltaEntry {
        Addr prev_miss_addr;
        Addr miss_pc;
        Addr miss_addr;
        std::vector<DeltaEntry>::iterator next;
        DeltaEntry() : prev_miss_addr(0), miss_pc(0), miss_addr(0) {}
    };

  protected:

    static const int Max_Masters = 64;

    Addr secondLastMissAddr[Max_Masters];
    Addr lastMissAddr[Max_Masters];

    std::map<long long, std::vector<DeltaEntry>::iterator> index_tables[Max_Masters];
    std::vector<DeltaEntry> ghbs[Max_Masters];
    std::vector<DeltaEntry>::iterator heads[Max_Masters];

    std::vector<DeltaEntry>::iterator insertEntry(Addr pc, Addr blk_addr, MasterID master_id);
    std::vector<DeltaEntry>::iterator addressListEnd(std::vector<DeltaEntry>::iterator it, MasterID master_id);
    std::vector<DeltaEntry>::iterator deltaCorrelation(std::vector<DeltaEntry>::iterator entry, MasterID master_id);
    std::pair<bool, long long> getPreviousDelta(std::vector<DeltaEntry>::iterator it, MasterID master_id);

  public:
    GHBPrefetcher(const Params *p)
        : BasePrefetcher(p)
    {
      for(int i = 0; i < Max_Masters; i++) {
        for(int j = 0; j < 512; j++)
        {
          DeltaEntry new_entry = {};
          ghbs[i].push_back(new_entry);
        }

        heads[i] = ghbs[i].begin();
      }
    }

    ~GHBPrefetcher() {}

    void calculatePrefetch(PacketPtr &pkt, std::list<Addr> &addresses,
                           std::list<Cycles> &delays);
};

#endif // __MEM_CACHE_PREFETCH_GHB_PREFETCHER_HH__
