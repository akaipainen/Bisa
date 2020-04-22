#pragma once

#include <Bisa.h>

class FirstHitOnStrip : public Bisa::Feature
{
public:
    FirstHitOnStrip()
    {
    }

    ~FirstHitOnStrip()
    {
    }
};

class FirstHitOnStripSelector : public Bisa::HitSelector
{
public:
    FirstHitOnStripSelector() = default;

    Bisa::HitCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::HitCollection hc; // output list of features

        std::unordered_map<int, Bisa::Ref<Bisa::Hit>> hit_strips_;

        for (auto &&hit : filterHits)
        {
            int uniqueStrip = hit.second->tdc * 32 + hit.second->channel;
            auto itHit = hit_strips_.find(uniqueStrip);
            if (itHit == hit_strips_.end())
            {
                hit_strips_[uniqueStrip] = hit.second;
            }
            else if (time(*hit.second) < time(*itHit->second))
            {
                hit_strips_[uniqueStrip] = hit.second;
            }
        }

        for (auto &&hitPair : hit_strips_)
        {
            hc.add(hitPair.second);
        }
        
        return hc;
    }

private:
    double time(const Bisa::Hit& hit) const
    {
        return hit.bcidTdc * 25 + hit.fineTime * 25.0/128.0;
    }
};
