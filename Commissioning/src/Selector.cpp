#include "Selector.h"

namespace Selector
{
    Bisa::FeatureCollection basicSelector(const Bisa::HitCollection& hits, const Bisa::Config &config, std::function<bool(const Bisa::Hit&, const Bisa::Hit&, const Bisa::Config&)> filter_pred)
    {
        Bisa::FeatureCollection fc; // output list of features

        std::deque<Bisa::HitCollection> featureList;
        std::deque<Bisa::Hit> remainingHits;

        // Initialize hits deque (need ordered container)
        for (auto &&hit : hits)
        {
            remainingHits.push_back(hit);
        }

        // Form initial cluster
        if (remainingHits.size() != 0)
        {
            Bisa::HitCollection feature;
            Bisa::Hit hit = *remainingHits.begin();
            remainingHits.pop_front();
            feature.add(hit);
            featureList.push_back(feature);
        }
        else return fc;

        // Loop over all digits to see if any can be added to a feature
        unsigned int notAddedCounter = 0;
        while (remainingHits.size() > 0)
        {
            // Process first hit in hits deque
            auto hit = *remainingHits.begin();
            // Loop through all hits in last cluster to check if
            // it satisfies the condition
            auto itHit = featureList.back().begin();
            for (; itHit != featureList.back().end(); itHit++)
            {
                // To add hit to the cluster, it must be stacked adjacent to 
                // and within 10 ns of a hit in the candidate
                if (filter_pred(hit, *itHit, config))
                {
                    // If hit is around another hit, add it to the cluster
                    featureList.back().add(hit);
                    remainingHits.pop_front();
                    notAddedCounter = 0;
                    break;
                }
            }
            // No digit can be added to this cluster
            if (itHit == featureList.back().end())
            {
                // If have checked all digits and none can be added to this cluster,
                // Create a new cluster
                if (++notAddedCounter == remainingHits.size())
                {
                    Bisa::HitCollection feature;
                    feature.add(hit);
                    remainingHits.pop_front();
                    featureList.push_back(feature);
                    notAddedCounter = 0;
                }
                // This digit cannot be added, cycle to next digit
                else 
                {
                    remainingHits.push_back(hit);
                    remainingHits.pop_front();
                }
            }
        }

        // Fill fc
        for (auto &&f : featureList)
        {
            fc.add(f);
        }
        
        // Return the FeatureCollection
        return fc;
    }

    Bisa::FeatureCollection requireMinSize(const Bisa::FeatureCollection& features, unsigned int minSize, const Bisa::Config &config)
    {
        Bisa::FeatureCollection fc; // output list of features
        for (auto &&feature : features)
        {
            if (feature.size() >= minSize)
            {
                fc.add(feature);
            }
        }
        return fc;
    }

    Bisa::FeatureCollection requireMaxSize(const Bisa::FeatureCollection& features, unsigned int maxSize, const Bisa::Config &config)
    {
        Bisa::FeatureCollection fc; // output list of features
        for (auto &&feature : features)
        {
            if (feature.size() <= maxSize)
            {
                fc.add(feature);
            }
        }
        return fc;
    }

    Bisa::FeatureCollection requireMultipleLayers(const Bisa::FeatureCollection& features, unsigned int nLayers, const Bisa::Config &config)
    {
        Bisa::FeatureCollection fc;
        for (auto &&feature : features)
        {
            std::set<int> layers;
            for (auto &&hit : feature)
            {
                layers.insert(config.layer(hit.tdc()));
            }
            if (layers.size() >= nLayers)
            {
                fc.add(feature);
            }
        }
        return fc;
    }

    Bisa::FeatureCollection restrictMaxHitsPerLayer(const Bisa::FeatureCollection& features, unsigned int maxSize, const Bisa::Config &config)
    {
        Bisa::FeatureCollection fc;
        for (auto &&feature : features)
        {
            std::map<int, int> layerHits;
            for (auto &&hit : feature)
            {
                layerHits[config.layer(hit.tdc())]++;
            }

            bool skip = false;
            for (auto &&layer_hits : layerHits)
            {
                if (layer_hits.second > maxSize)
                {
                    skip = true;
                    return Bisa::FeatureCollection(); // skip entire event
                }
            }

            if (!skip)
            {
                fc.add(feature);
            }
        }
        return fc;
    }

}
