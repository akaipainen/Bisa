#pragma once

#include "bapch.h"

#include "Bisa/Features/HitCollection.h"

namespace Bisa {

    template <typename F>
    class BISA_API FeatureCollection
    {
    public:
        using Iterator = typename std::vector<F>::iterator;
        using ConstIterator = typename std::vector<F>::const_iterator;

    public:
        // Default constructor
        FeatureCollection() = default;

        // Add a single feature to this collection
        void add(const F& feature)
        {
            BA_CORE_ASSERT(feature.trigger_id() == trigger_id_ || size() == 0, 
                        "Adding Feature with different Trigger ID to FeatureCollection");
            features_.emplace_back(feature);
            trigger_id_ = feature.trigger_id();
        }

        // Return the number of features in this collection
        unsigned int size() const { return features_.size(); }

        // Return a F of all hits in this feature collection
        HitCollection hits()
        {
            HitCollection hc;
            for (auto &&feature : features_)
            {
                hc = hc | feature;
            }
            return hc;
        }

        // Return string interpretation of this object (debug tool)
        ::std::string to_string() const
        {
            std::stringstream ret;
            ret << "Trigger ID: " << trigger_id_;
            ret << ", Number of features: " << size();
            return ret.str();
        }

        // Return begin iterator (non-const)
        Iterator begin() { return features_.begin(); }
        
        // Return end iterator (non-const)
        Iterator end() { return features_.end(); }

        // Return begin iterator (const)
        ConstIterator begin() const { return features_.begin(); }
        
        // Return end iterator (const)
        ConstIterator end() const { return features_.end(); }

    private:
        ::std::vector<F> features_;

        unsigned int trigger_id_;
    };
}
