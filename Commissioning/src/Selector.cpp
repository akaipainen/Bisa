#include "Selector.h"

namespace Selector
{
    Bisa::FeatureCollection<Bisa::Cluster> clusterize(const Bisa::HitCollection &hits)
    {
        std::deque<Bisa::Cluster> clusters;
        std::deque<Bisa::Hit> remaining_hits;

        // Initialize hits deque (need ordered container)
        for (auto &&hit : hits) remaining_hits.push_back(hit);

        // Form initial cluster
        if (remaining_hits.size() != 0)
        {
            Bisa::Cluster cluster;
            Bisa::Hit hit = *remaining_hits.begin();
            remaining_hits.pop_front();
            cluster.add(hit);
            clusters.push_back(cluster);
        }
        else return Bisa::FeatureCollection<Bisa::Cluster>();

        // Loop over all digits to see if any can be added to a feature
        unsigned int not_added_counter = 0;
        while (remaining_hits.size() > 0)
        {
            // Process first hit in hits deque
            auto hit = *remaining_hits.begin();
            // Loop through all hits in last cluster to check if
            // it satisfies the condition
            auto hit_it = clusters.back().begin();
            for (; hit_it != clusters.back().end(); hit_it++)
            {
                if (hit.chamber() == hit_it->chamber() &&
                    hit.coordinate() == hit_it->coordinate() &&
                    hit.layer() == hit_it->layer() &&
                    std::abs((int) hit.strip() - (int) hit_it->strip()) <= 4 &&
                    std::abs(hit.time() - hit_it->time()) <= 5)
                {
                    clusters.back().add(hit);
                    remaining_hits.pop_front();
                    not_added_counter = 0;
                    break;
                }
            }
            // No digit can be added to this cluster
            if (hit_it == clusters.back().end())
            {
                // If have checked all digits and none can be added to this cluster,
                // Create a new cluster
                if (++not_added_counter == remaining_hits.size())
                {
                    Bisa::Cluster cluster;
                    cluster.add(hit);
                    remaining_hits.pop_front();
                    clusters.push_back(cluster);
                    not_added_counter = 0;
                }
                // This digit cannot be added, cycle to next digit
                else 
                {
                    remaining_hits.push_back(hit);
                    remaining_hits.pop_front();
                }
            }
        }

        // Fill fc
        Bisa::FeatureCollection<Bisa::Cluster> fc;
        for (auto &&c : clusters)
        {
            fc.add(c);
        }
        
        // Return the FeatureCollection
        return fc;
    }

    // Bisa::FeatureCollection basicSelector(const Bisa::HitCollection& hits, const Bisa::Config &config, std::function<bool(const Bisa::Hit&, const Bisa::Hit&, const Bisa::Config&)> filter_pred)
    // {
    //     Bisa::FeatureCollection fc; // output list of features

    //     std::deque<Bisa::HitCollection> featureList;
    //     std::deque<Bisa::Hit> remaining_hits;

    //     // Initialize hits deque (need ordered container)
    //     for (auto &&hit : hits)
    //     {
    //         remaining_hits.push_back(hit);
    //     }

    //     // Form initial cluster
    //     if (remaining_hits.size() != 0)
    //     {
    //         Bisa::HitCollection feature;
    //         Bisa::Hit hit = *remaining_hits.begin();
    //         remaining_hits.pop_front();
    //         feature.add(hit);
    //         featureList.push_back(feature);
    //     }
    //     else return fc;

    //     // Loop over all digits to see if any can be added to a feature
    //     unsigned int notAddedCounter = 0;
    //     while (remaining_hits.size() > 0)
    //     {
    //         // Process first hit in hits deque
    //         auto hit = *remaining_hits.begin();
    //         // Loop through all hits in last cluster to check if
    //         // it satisfies the condition
    //         auto hit_it = featureList.back().begin();
    //         for (; hit_it != featureList.back().end(); hit_it++)
    //         {
    //             // To add hit to the cluster, it must be stacked adjacent to 
    //             // and within 10 ns of a hit in the candidate
    //             if (filter_pred(hit, *hit_it, config))
    //             {
    //                 // If hit is around another hit, add it to the cluster
    //                 featureList.back().add(hit);
    //                 remaining_hits.pop_front();
    //                 notAddedCounter = 0;
    //                 break;
    //             }
    //         }
    //         // No digit can be added to this cluster
    //         if (hit_it == featureList.back().end())
    //         {
    //             // If have checked all digits and none can be added to this cluster,
    //             // Create a new cluster
    //             if (++notAddedCounter == remaining_hits.size())
    //             {
    //                 Bisa::HitCollection feature;
    //                 feature.add(hit);
    //                 remaining_hits.pop_front();
    //                 featureList.push_back(feature);
    //                 notAddedCounter = 0;
    //             }
    //             // This digit cannot be added, cycle to next digit
    //             else 
    //             {
    //                 remaining_hits.push_back(hit);
    //                 remaining_hits.pop_front();
    //             }
    //         }
    //     }

    //     // Fill fc
    //     for (auto &&f : featureList)
    //     {
    //         fc.add(f);
    //     }
        
    //     // Return the FeatureCollection
    //     return fc;
    // }

    // Bisa::FeatureCollection requireMinSize(const Bisa::FeatureCollection& features, unsigned int minSize, const Bisa::Config &config)
    // {
    //     Bisa::FeatureCollection fc; // output list of features
    //     for (auto &&feature : features)
    //     {
    //         if (feature.size() >= minSize)
    //         {
    //             fc.add(feature);
    //         }
    //     }
    //     return fc;
    // }

    // Bisa::FeatureCollection requireMaxSize(const Bisa::FeatureCollection& features, unsigned int maxSize, const Bisa::Config &config)
    // {
    //     Bisa::FeatureCollection fc; // output list of features
    //     for (auto &&feature : features)
    //     {
    //         if (feature.size() <= maxSize)
    //         {
    //             fc.add(feature);
    //         }
    //     }
    //     return fc;
    // }

    // Bisa::FeatureCollection requireMultipleLayers(const Bisa::FeatureCollection& features, unsigned int nLayers, const Bisa::Config &config)
    // {
    //     Bisa::FeatureCollection fc;
    //     for (auto &&feature : features)
    //     {
    //         std::set<int> layers;
    //         for (auto &&hit : feature)
    //         {
    //             layers.insert(config.layer(hit.tdc()));
    //         }
    //         if (layers.size() >= nLayers)
    //         {
    //             fc.add(feature);
    //         }
    //     }
    //     return fc;
    // }

    // Bisa::FeatureCollection restrictMaxHitsPerLayer(const Bisa::FeatureCollection& features, unsigned int maxSize, const Bisa::Config &config)
    // {
    //     Bisa::FeatureCollection fc;
    //     for (auto &&feature : features)
    //     {
    //         std::map<int, int> layerHits;
    //         for (auto &&hit : feature)
    //         {
    //             layerHits[config.layer(hit.tdc())]++;
    //         }

    //         bool skip = false;
    //         for (auto &&layer_hits : layerHits)
    //         {
    //             if (layer_hits.second > maxSize)
    //             {
    //                 skip = true;
    //                 return Bisa::FeatureCollection(); // skip entire event
    //             }
    //         }

    //         if (!skip)
    //         {
    //             fc.add(feature);
    //         }
    //     }
    //     return fc;
    // }

}
