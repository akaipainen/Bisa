#pragma once

#include <Bisa.h>

class LayerSpaceCluster : public Bisa::Feature
{
public:
    LayerSpaceCluster()
    {

    }

    ~LayerSpaceCluster()
    {
    }
};

class LayerSpaceClusterSelector : public Bisa::FeatureSelector
{
public:
    LayerSpaceClusterSelector(const Bisa::Config& config)
     : Bisa::FeatureSelector(config)
    {
    }

    Bisa::FeatureCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::FeatureCollection fc; // output list of features

        // BA_TRACE("Initializing variables");

        std::deque<Bisa::Ref<LayerSpaceCluster>> featureList;
        std::deque<Bisa::Ref<Bisa::Hit>> hits;
        // Initialize hits deque (need ordered container)
        for (auto hit_it = filterHits.begin(); hit_it != filterHits.end(); hit_it++)
        {
            hits.push_back(hit_it.get());
        }

        // Form initial cluster
        // BA_TRACE("Form initial cluster");
        if (hits.size() != 0)
        {
            Bisa::Ref<LayerSpaceCluster> cluster = Bisa::CreateRef<LayerSpaceCluster>();
            Bisa::Ref<Bisa::Hit> hit = *hits.begin();
            cluster->hits().add(hit);
            hits.pop_front();
            featureList.push_back(cluster);
        }
        else return fc;

        // Loop over all digits to if any can be added to created cluster
        // BA_TRACE("Loop over all digits to see if any can be added to created cluster");
        unsigned int notAddedCounter = 0;
        while (hits.size() > 0)
        {
            // Process first hit in hits deque
            // BA_TRACE("Process first hit in hits deque");
            auto hit = *hits.begin();
            // Loop through all hits in last cluster to check if
            // it satisfies the condition
            // BA_TRACE("Loop through all hits");
            auto itHit = featureList.back()->hits().begin();
            for (; itHit != featureList.back()->hits().end(); itHit++)
            {
                if (around(*hit, *itHit))
                {
                    // If hit is around another hit, add it to the cluster
                    // BA_TRACE("Adding hit to cluster");
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
                    auto cluster = Bisa::CreateRef<LayerSpaceCluster>();
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

        // BA_TRACE("Finished creating ({}) space clusters", featureList.size());


        // Fill fc
        for (auto &&f : featureList)
        {
            fc.add(f);
        }

        // BA_TRACE("Size: {}", fc.size());
        
        // Return the FeatureCollection
        return fc;
    }

private:
    bool around(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        // Same chamber
        if (config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()))
        {
            // Same coordinate
            if (config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
            {
                // Same layer
                if (config_.layer(hit1.tdc()) == config_.layer(hit2.tdc()))
                {
                    // Maximum 1 strip apart
                    if (std::abs(config_.strip(hit1.channel()) - config_.strip(hit2.channel())) <= 1)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};
