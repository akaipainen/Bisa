#pragma once

#include <Bisa.h>

#include "HitDistribution.h"

class MuonHitsDistribution : public HitDistribution
{
public:
    MuonHitsDistribution(const char* name, const Bisa::Config& config)
     : HitDistribution(name, config)
    {
    }

    ~MuonHitsDistribution()
    {
        extra_configure();
    }

    void extra_configure()
    {
        strip_rate_.for_each([&] (TH1F& hist) {
            hist.Scale(1/config_.run_duration());
        });
        channel_rate_.for_each([&] (TH1F& hist) {
            hist.Scale(1/config_.run_duration());
        });
    }

};