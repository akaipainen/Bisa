#pragma once

#include <Bisa.h>

class NoiseBurst : public Bisa::Feature
{
public:
    NoiseBurst()
    {
    }

    ~NoiseBurst()
    {
    }
};

class NoiseBurstSelector : public Bisa::FeatureSelector
{
public:
    NoiseBurstSelector(const Bisa::Config& config)
     : Bisa::FeatureSelector(config)
    {
    }

    Bisa::FeatureCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::FeatureCollection fc; // output list of features

        std::deque<Bisa::Ref<NoiseBurst>> featureList;
        std::deque<Bisa::Ref<Bisa::Hit>> hits;

        // Initialize hits deque (need ordered container)
        for (auto itHit = filterHits.begin(); itHit != filterHits.end(); itHit++)
        {
            hits.push_back(itHit.get());
        }

        // Form initial cluster
        if (hits.size() != 0)
        {
            Bisa::Ref<NoiseBurst> cluster = Bisa::CreateRef<NoiseBurst>();
            Bisa::Ref<Bisa::Hit> hit = *hits.begin();
            cluster->hits().add(hit);
            hits.pop_front();
            featureList.push_back(cluster);
        }
        else return fc;

        // Loop over all digits to if any can be added to created cluster
        unsigned int notAddedCounter = 0;
        while (hits.size() > 0)
        {
            // Process first hit in hits deque
            auto hit = *hits.begin();
            // Loop through all hits in last cluster to check if
            // it satisfies the condition
            auto itHit = featureList.back()->hits().begin();
            for (; itHit != featureList.back()->hits().end(); itHit++)
            {
                // To add hit to the cluster, it must be adjacent to 
                // and within 10 ns of a hit in the cluster
                if (adjacent(*hit, *itHit) &&
                    std::abs(time(*hit) - time(*itHit)) < 10)
                {
                    // If hit is around another hit, add it to the cluster
                    featureList.back()->hits().add(hit);
                    hits.pop_front();
                    notAddedCounter = 0;
                    break;
                }
            }
            // No digit can be added to this cluster
            if (itHit == featureList.back()->hits().end())
            {
                // If have checked all digits and none can be added to this cluster,
                // Create a new cluster
                if (++notAddedCounter == hits.size())
                {
                    auto cluster = Bisa::CreateRef<NoiseBurst>();
                    cluster->hits().add(hit);
                    hits.pop_front();
                    featureList.push_back(cluster);
                    notAddedCounter = 0;
                }
                // This digit cannot be added, cycle to next digit
                else 
                {
                    hits.push_back(hit);
                    hits.pop_front();
                }
            }
        }

        // Fill fc
        for (auto &&f : featureList)
        {
            // Only consider this a noise burst if there are 8 or more hits
            // in the cluster
            if (f->hits().size() >= 8)
            {
                fc.add(f);
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
                                 config_.orientation(hit2.tdc())*32+config_.strip(hit1.channel())) == 1)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool stacked(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        // Same chamber
        if (config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()))
        {
            // Same coordinate
            if (config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
            {
                // Same orientation
                if (config_.orientation(hit1.tdc()) == config_.orientation(hit2.tdc()))
                {
                    // Same channel/strip
                    return hit1.channel() == hit2.channel();
                }
            }
        }
        return false;
    }

    bool around(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        return stacked(hit1, hit2) || adjacent(hit1, hit2);
    }

    double time(const Bisa::Hit& hit) const
    {
        return hit.bcid_tdc() * config_.bcid_resolution() + hit.fine_time() * config_.fine_time_resolution();
    }
};
