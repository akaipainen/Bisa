#pragma once

#include <Bisa.h>

class TimeCluster : public Bisa::Feature
{
public:
    TimeCluster()
    {
    }

    ~TimeCluster()
    {
    }
};

class TimeClusterSelector : public Bisa::FeatureSelector
{
public:
    TimeClusterSelector(const Bisa::Config& config)
     : Bisa::FeatureSelector(config)
    {
    }

    Bisa::FeatureCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::FeatureCollection fc; // output list of features

        std::deque<Bisa::Ref<TimeCluster>> featureList;
        std::deque<Bisa::Ref<Bisa::Hit>> hits;

        // Initialize hits deque (need ordered container)
        for (auto itHit = filterHits.begin(); itHit != filterHits.end(); itHit++)
        {
            hits.push_back(itHit.get());
        }

        // Form initial cluster
        if (hits.size() != 0)
        {
            Bisa::Ref<TimeCluster> cluster = Bisa::CreateRef<TimeCluster>();
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
                if (std::abs(time(*hit) - time(*itHit)) < 10)
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
                    auto cluster = Bisa::CreateRef<TimeCluster>();
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
            if (f->hits().size() > 1) fc.add(f);
        }
        
        // Return the FeatureCollection
        return fc;
    }

private:
    double time(const Bisa::Hit& hit) const
    {
        return hit.bcid_tdc() * config_.bcid_resolution() + hit.fine_time() * config_.fine_time_resolution();
    }
};
