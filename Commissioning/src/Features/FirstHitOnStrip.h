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
    FirstHitOnStripSelector(const Bisa::Config& config)
     : Bisa::HitSelector(config)
    {
    }

    Bisa::HitCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::HitCollection hc; // output list of features

        std::unordered_map<int, Bisa::Ref<Bisa::Hit>> hit_strips_;

        for (auto hit_iter = filterHits.begin(); hit_iter != filterHits.end(); hit_iter++)
        {
            int unique_strip = hit_iter->tdc() * 32 + hit_iter->channel();
            auto find_hit = hit_strips_.find(unique_strip);
            if (find_hit == hit_strips_.end())
            {
                hit_strips_[unique_strip] = hit_iter.get();
            }
            else if (time(*hit_iter) < time(*find_hit->second))
            {
                hit_strips_[unique_strip] = hit_iter.get();
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
        return hit.bcid_tdc() * config_.bcid_resolution() + hit.fine_time() * config_.fine_time_resolution();
    }
};
