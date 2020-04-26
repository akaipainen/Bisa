#pragma once

#include <Bisa.h>

#include "HitDistribution.h"

class NoiseBurstDistribution : public HitDistribution
{
public:
    NoiseBurstDistribution(const char* name, const Bisa::Config& config)
     : HitDistribution(name, config)
    {
    }

    ~NoiseBurstDistribution()
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