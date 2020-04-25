#pragma once

#include <Bisa.h>

#include "HitDistribution.h"

class MuonHitsDistribution : public HitDistribution
{
public:
    MuonHitsDistribution(const char* name, double run_duration)
     : HitDistribution(name)
     , run_duration_(run_duration)
    {
    }

    ~MuonHitsDistribution()
    {
        extra_configure();
    }

    void extra_configure()
    {
        strip_rate_.for_each([&] (TH1F& hist) {
            hist.Scale(1/(run_duration_));
        });
        channel_rate_.for_each([&] (TH1F& hist) {
            hist.Scale(1/(run_duration_));
        });
    }

private:
    double run_duration_;
};