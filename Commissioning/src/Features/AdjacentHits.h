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
    AdjacentHitsSelector(const Bisa::Config& config)
     : Bisa::FeatureSelector(config)
    {
    }

    Bisa::FeatureCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::FeatureCollection fc; // output list of features

        for (auto hit1_iter = filterHits.begin(); hit1_iter != filterHits.end(); hit1_iter++)
        {
            for (auto hit2_iter = filterHits.begin(); hit2_iter != filterHits.end(); hit2_iter++)
            {
                if (*hit1_iter != *hit2_iter)
                {
                    if (adjacent(*hit1_iter, *hit2_iter))
                    {
                        auto adjhits = Bisa::CreateRef<AdjacentHits>();
                        adjhits->hits().add(hit1_iter.get());
                        adjhits->hits().add(hit2_iter.get());
                        fc.add(adjhits);
                    }
                }
            }
        }

        // Return the FeatureCollection
        return fc;
    }

    Bisa::FeatureCollection operator()(const Bisa::HitCollection& filterHits, int direction=0) const
    {
        Bisa::FeatureCollection fc; // output list of features

        for (auto hit1_iter = filterHits.begin(); hit1_iter != filterHits.end(); hit1_iter++)
        {
            for (auto hit2_iter = filterHits.begin(); hit2_iter != filterHits.end(); hit2_iter++)
            {
                if (*hit1_iter != *hit2_iter)
                {
                    auto adjhits = Bisa::CreateRef<AdjacentHits>();
                    if (orientation(*hit1_iter, *hit2_iter) == 1)
                    {
                        if (direction == 1) // Has right adjacent
                        {
                            adjhits->hits().add(hit1_iter.get());
                            fc.add(adjhits);
                        }
                        if (direction == -1) // Has left adjacent
                        {
                            adjhits->hits().add(hit2_iter.get());
                            fc.add(adjhits);
                        }
                    }
                }
            }
        }

        // Return the FeatureCollection
        return fc;
    }

private:
    bool adjacent(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        if (hit1.tdc() == hit2.tdc()) 
            return std::abs(config_.strip(hit1.channel()) - config_.strip(hit2.channel())) == 1;

        // Same chamber
        if (config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()))
        {
            // Same coordinate
            if (config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
            {
                // Same layer
                if (config_.layer(hit1.tdc()) == config_.layer(hit2.tdc()))
                {
                    // Adjacent strips across layer
                    if (std::abs(config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel()) - 
                                 config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel())) == 1)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    int orientation(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        if (hit1.tdc() == hit2.tdc()) 
            return config_.strip(hit2.channel()) - config_.strip(hit1.channel());

        // Same chamber
        if (config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()))
        {
            // Same coordinate
            if (config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
            {
                // Same layer
                if (config_.layer(hit1.tdc()) == config_.layer(hit2.tdc()))
                {
                    // Adjacent strips across layer
                    int dist = (config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel())) - (config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel()));
                    return dist;
                }
            }
        }
        return 0;
    }
};
