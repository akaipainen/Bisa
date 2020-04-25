#pragma once

#include <Bisa.h>

class AdjacentHits : public Bisa::Feature
{
public:
    AdjacentHits()
    {

    }

    ~AdjacentHits()
    {
    }
};

class AdjacentHitsSelector : public Bisa::FeatureSelector
{
public:
    AdjacentHitsSelector() = default;

    Bisa::FeatureCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::FeatureCollection fc; // output list of features

        for (auto &&hit1 : filterHits)
        {
            for (auto &&hit2 : filterHits)
            {
                if (hit1 != hit2)
                {
                    if (adjacent(*hit1.second, *hit2.second))
                    {
                        auto adjhits = Bisa::CreateRef<AdjacentHits>();
                        adjhits->hits().add(hit1.second);
                        adjhits->hits().add(hit2.second);
                        fc.add(adjhits);
                    }
                }
            }
        }

        // Return the FeatureCollection
        return fc;
    }

private:
    // TODO: Replace fixed values for constant functions with configuration service
    bool adjacent(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        if (hit1.tdc == hit2.tdc) 
            return std::abs(strip_mapping[hit1.channel] - strip_mapping[hit2.channel]) == 1;
        
        else if ((hit1.tdc == 3 && hit2.tdc == 4) ||
                 (hit1.tdc == 5 && hit2.tdc == 6) ||
                 (hit1.tdc == 7 && hit2.tdc == 8))
            return strip_mapping[hit1.channel] == 31 && strip_mapping[hit2.channel] == 0;
        else if ((hit1.tdc == 4 && hit2.tdc == 3) ||
                 (hit1.tdc == 6 && hit2.tdc == 5) ||
                 (hit1.tdc == 8 && hit2.tdc == 7))
            return strip_mapping[hit1.channel] == 0 && strip_mapping[hit2.channel] == 31;
        else
            return false;
    }
};
