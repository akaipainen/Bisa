#pragma once

#include <Bisa.h>

class MuonCandidate : public Bisa::Feature
{
public:
    MuonCandidate()
    {
    }

    ~MuonCandidate()
    {
    }
};

class MuonCandidateSelector : public Bisa::FeatureSelector
{
public:
    MuonCandidateSelector(const Bisa::Config& config)
     : Bisa::FeatureSelector(config)
    {
    }

    Bisa::FeatureCollection operator()(const Bisa::HitCollection& filterHits) const
    {
        Bisa::FeatureCollection fc; // output list of features

        std::deque<Bisa::Ref<MuonCandidate>> featureList;
        std::deque<Bisa::Ref<Bisa::Hit>> hits;

        // Initialize hits deque (need ordered container)
        for (auto itHit = filterHits.begin(); itHit != filterHits.end(); itHit++)
        {
            hits.push_back(itHit.get());
        }

        // Form initial cluster
        if (hits.size() != 0)
        {
            Bisa::Ref<MuonCandidate> cluster = Bisa::CreateRef<MuonCandidate>();
            Bisa::Ref<Bisa::Hit> hit = *hits.begin();
            cluster->hits().add(hit);
            hits.pop_front();
            featureList.push_back(cluster);
        }
        else return fc;

        // Loop over all digits to see if any can be added to created cluster
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
                // To add hit to the cluster, it must be stacked adjacent to 
                // and within 10 ns of a hit in the candidate
                auto sa = stacked_adjacent(
                    *hit, *itHit);
                auto td = std::abs(time(*hit) - time(*itHit));
                if (stacked_adjacent(*hit, *itHit) &&
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
                    auto cluster = Bisa::CreateRef<MuonCandidate>();
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
            if (f->hits().size() > 1)
            {
                fc.add(f);
            }
        }
        
        // Return the FeatureCollection
        return fc;
    }

private:
    bool stacked_adjacent(const Bisa::Hit& hit1, const Bisa::Hit& hit2) const
    {
        // Same chamber
        if (config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()))
        {
            // Same coordinate
            if (config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
            {
                // 1 or 2 layers apart
                if (std::abs(config_.layer(hit1.tdc()) - config_.layer(hit2.tdc())) >= 1)
                {
                    // 2 or less strips apart
                    auto a = config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel());
                    auto b = config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel());
                    auto c = std::abs((config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel())) - (config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel())));
                    if (std::abs((config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel())) - (config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel()))) <= 2)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    double time(const Bisa::Hit& hit) const
    {
        return hit.bcid_tdc() * config_.bcid_resolution() + hit.fine_time() * config_.fine_time_resolution();
    }
};
