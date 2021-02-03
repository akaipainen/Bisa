#include "FeatureCollection.h"

namespace Bisa {

    void FeatureCollection::add(const HitCollection& feature)
    {
        BA_CORE_ASSERT(feature.trigger_id() == trigger_id_ || size() == 0, 
                       "Adding Feature with different Trigger ID to FeatureCollection");
        features_.emplace_back(feature);
        trigger_id_ = feature.trigger_id();
    }

    HitCollection FeatureCollection::hits()
    {
        HitCollection hc;
        for (auto &&feature : features_)
        {
            hc = hc | feature;
        }
        return hc;
    }

    std::string FeatureCollection::to_string() const
    {
        std::stringstream ret;
        ret << "Trigger ID: " << trigger_id_;
        ret << ", Number of features: " << size();
        return ret.str();
    }

}