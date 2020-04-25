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

        for (auto hit_iter = filterHits.begin(); hit_iter != filterHits.end(); hit_iter++)
        {
            int uniqueStrip = hit_iter->tdc() * 32 + hit_iter->channel();
            auto itHit = hit_strips_.find(uniqueStrip);
            if (itHit == hit_strips_.end())
            {
                hit_strips_[uniqueStrip] = hit_iter.get();
            }
            else if (time(*hit_iter) < time(*itHit->second))
            {
                hit_strips_[uniqueStrip] = hit_iter.get();
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
        return hit.bcid_tdc() * 25 + hit.fine_time() * 25.0/128.0;
    }
};
