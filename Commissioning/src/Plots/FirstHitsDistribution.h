#pragma once

#include <Bisa.h>

#include "HitDistribution.h"

class FirstHitsDistribution : public HitDistribution
{
public:
    FirstHitsDistribution(const char* name, const Bisa::Config& config)
     : HitDistribution(name, config)
    {
    }

    ~FirstHitsDistribution()
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