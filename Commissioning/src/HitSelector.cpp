#include "HitSelector.h"

#include "SelectorFilter.h"

Bisa::HitCollection HitSelector::firstHits(const Bisa::HitCollection& hits)
{
    Bisa::HitCollection hc;
    std::unordered_map<int, Bisa::Hit> hit_set;
    for (auto &&hit : hits)
    {
        auto hit_it = hit_set.find(32 * hit.tdc() + hit.channel());
        if (hit_it == hit_set.end())
        {
            hit_set[32 * hit.tdc() + hit.channel()] = hit;
        } else if (config_.time(hit.bcid_tdc(), hit.fine_time()) < config_.time(hit_it->second.bcid_tdc(), hit_it->second.fine_time())) {
            hit_set[32 * hit.tdc() + hit.channel()] = hit;
        }
    }
    for (auto &&hit_it : hit_set)
    {
        hc.add(hit_it.second);
    }
    
    return hc;
}

Bisa::HitCollection HitSelector::featureCorrelated(const Bisa::HitCollection& hits, 
                                                   const Bisa::FeatureCollection& features, 
                                                   std::function<bool(const Bisa::Hit&, const Bisa::Hit&, const Bisa::Config&)> filter_pred)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        for (auto &&feature : features)
        {
            auto itTestHit = feature.begin();
            for (; itTestHit != feature.end(); itTestHit++)
            {
                if (filter_pred(hit, *itTestHit, config_))
                {
                    hc.add(hit);
                    break;
                }
            }
            // We found it if we are not at end iterator
            if (itTestHit != feature.end())
            {
                break;
            }
        }
    }
    return hc;
}

Bisa::HitCollection HitSelector::hasHitToRight(const Bisa::HitCollection& hits)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        for (auto &&testHit : hits)
        {
            if (hit != testHit)
            {
                if (utils::hits_signed_dist_apart(hit, testHit, config_) == 1)
                {
                    hc.add(hit);
                }
            }
        }
    }
    return hc;
}

Bisa::HitCollection HitSelector::hasHitToLeft(const Bisa::HitCollection& hits)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        for (auto &&testHit : hits)
        {
            if (hit != testHit)
            {
                if (utils::hits_same_coord_layer(hit, testHit, config_))
                {
                    if (utils::hits_signed_dist_apart(hit, testHit, config_) == 1)
                    {
                        hc.add(testHit);
                    }
                }
            }
        }
    }
    return hc;
}

Bisa::HitCollection HitSelector::restrictHitsOnLayer(const Bisa::HitCollection& hits, unsigned int nHits)
{
    Bisa::HitCollection hc;
    std::vector<int> hits_on_layer_(12, 0);
    for (auto &&hit : hits)
    {
        hits_on_layer_[6 * (config_.chamber(hit.tdc()) - 7) + 3 * config_.coordinate(hit.tdc()) + config_.layer(hit.tdc())]++;
    }
    for (auto &&hit : hits)
    {
        if (hits_on_layer_[6 * config_.chamber(hit.tdc()) + 3 * config_.coordinate(hit.tdc()) + config_.layer(hit.tdc())] <= nHits)
        {
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection HitSelector::restrictHitsOnTDC(const Bisa::HitCollection& hits, unsigned int nHits)
{
    Bisa::HitCollection hc;
    std::vector<int> hits_on_tdc_(18, 0);
    for (auto &&hit : hits)
    {
        hits_on_tdc_[hit.tdc()]++;
    }
    for (auto &&hit : hits)
    {
        if (hits_on_tdc_[hit.tdc()] == nHits)
        {
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection HitSelector::equalWidth(const Bisa::HitCollection& hits, unsigned int width)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (hit.width() == width)
        {
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection HitSelector::smallestBcid(const Bisa::HitCollection& hits)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (hc.empty() || hit.bcid_fpga() == hc.begin()->bcid_fpga())
        {
            hc.add(hit);
        }
        else if (hit.bcid_fpga() < hc.begin()->bcid_fpga())
        {
            hc.clear();
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection  HitSelector::withinFirstFpgaTimestamp(const Bisa::HitCollection& hits, unsigned int nTicks)
{
    Bisa::HitCollection hc;
    unsigned int min_timestamp = hits.begin()->bcid_fpga();
    for (auto &&hit : hits)
    {
        if (hit.bcid_fpga() < min_timestamp)
        {
            min_timestamp = hit.bcid_fpga();
        }
    }
    for (auto &&hit : hits)
    {
        if (hit.bcid_fpga() <= min_timestamp + nTicks)
        {
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection HitSelector::phi(const Bisa::HitCollection& hits)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (config_.coordinate(hit.tdc()) == Bisa::PHI)
        {
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection HitSelector::eta(const Bisa::HitCollection& hits)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (config_.coordinate(hit.tdc()) == Bisa::ETA)
        {
            hc.add(hit);
        }
    }
    return hc;
}