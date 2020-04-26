#include "FeatureCollection.h"

namespace Bisa {

    void FeatureCollection::add(Ref<Feature> feature)
    {
        auto found = feature_ids_.find(feature->unique_id());
        if (found == feature_ids_.end())
        {
            features_.push_back(feature);
            feature_ids_.insert(feature->unique_id());
        }
    }

    void FeatureCollection::remove(Iterator it)
    {
        std::list<Ref<Feature>>::iterator pit = it;
        features_.erase(pit);
        feature_ids_.erase(it->unique_id());
    }

    // void FeatureCollection::remove(ConstIterator it)
    // {
    //     std::list<Ref<Feature>>::const_iterator pit = it;
    //     features_.erase(pit);
    // }

    void FeatureCollection::remove(Ref<Feature> feature)
    {
        // If this feature exists in this collection, find it and remove it
        auto search = feature_ids_.find(feature->unique_id());
        if (search != feature_ids_.end())
        {
            auto found = std::find(begin(), end(), *feature);
            remove(found);
        }
    }

    void FeatureCollection::add(const FeatureCollection& features)
    {
        for (auto feature_it = features.begin(); feature_it != features.end(); feature_it++)
        {
            add(feature_it.get());
        }
    }

    void FeatureCollection::remove(const FeatureCollection& features)
    {
        for (auto feature_it = features.begin(); feature_it != features.end(); feature_it++)
        {
            remove(feature_it.get());
        }
    }

    HitCollection FeatureCollection::hits()
    {
        HitCollection hc;
        for (auto &&feature : features_)
        {
            hc.add(feature->hits());
        }
        return hc;
    }

    FeatureCollection FeatureCollection::operator&(const FeatureCollection& other) const
    {
        FeatureCollection fc;
        for (auto &&feature1 : *this)
        {
            for (auto &&feature2 : other)
            {
                fc.add(CreateRef<Feature>(feature1.hits() & feature2.hits()));
            }
        }
        return fc;
    }

    std::string FeatureCollection::to_string() const
    {
        std::stringstream ret;
        ret << "Trigger ID: " << trigger_id_;
        ret << ", Number of features: " << size();
        return ret.str();
    }

}