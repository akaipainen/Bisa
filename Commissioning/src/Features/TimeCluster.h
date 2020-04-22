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
    TimeClusterSelector() = default;

    Bisa::FeatureCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::FeatureCollection fc; // output list of features

        std::deque<Bisa::Ref<TimeCluster>> featureList;
        std::deque<Bisa::Ref<Bisa::Hit>> hits;

        // Initialize hits deque (need ordered container)
        for (auto itHit = filterHits.begin(); itHit != filterHits.end(); itHit++)
        {
            hits.push_back(itHit->second);
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
                if (std::abs(time(*hit) - time(*itHit->second)) < 10)
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
    // TODO: Replace fixed values for constant functions with configuration service
    bool adjacent(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        const std::vector<int> strip_mapping = {
            0, 4, 8, 12, 16, 20, 24, 28,
            1, 5, 9, 13, 17, 21, 25, 29,
            2, 6,10, 14, 18, 22, 26, 30,
            3, 7,11, 15, 19, 23, 27, 31
        };
        
        if (hit1.tdc == hit2.tdc) 
            return std::abs(strip_mapping[hit1.channel] - strip_mapping[hit2.channel]) <= 1;
        
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

    bool stacked(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        if (hit1.tdc < 3 && hit2.tdc < 3) 
            return hit1.channel == hit2.channel;
        else if ((hit1.tdc >= 3 && hit1.tdc % 2 == 1) &&
                 (hit2.tdc >= 3 && hit2.tdc % 2 == 1))
            return hit1.channel == hit2.channel;
        else if ((hit1.tdc >= 3 && hit1.tdc % 2 == 0) &&
                 (hit2.tdc >= 3 && hit2.tdc % 2 == 0))
            return hit1.channel == hit2.channel;
        else
            return false;
    }

    bool around(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        return stacked(hit1, hit2) || adjacent(hit1, hit2);
    }

    double time(const Bisa::Hit& hit) const
    {
        return hit.bcidTdc * 25 + hit.fineTime * 25.0/128.0;
    }
};
