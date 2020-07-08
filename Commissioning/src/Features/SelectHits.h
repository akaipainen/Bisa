#pragma once

#include <Bisa.h>

class SelectHitsSelector
{
public:
    SelectHitsSelector(const Bisa::Config& config)
     : config_(config)
    {
    }

    Bisa::HitCollection operator()(const Bisa::HitCollection& filterHits, int tdc, int strip) const
    {

        Bisa::HitCollection hc; // filtered hits

        for (auto hit_iter = filterHits.begin(); hit_iter != filterHits.end(); hit_iter++)
        {
            if (hit_iter->tdc() == tdc && config_.strip(hit_iter->channel()) == strip)
            {
                    hc.add(hit_iter.get());
            }
        }

        // Return the FeatureCollection
        return hc;
    }

private:
    const Bisa::Config &config_;
};
