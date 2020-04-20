#pragma once

#include "bapch.h"

#include "Bisa/Features/HitCollection.h"
#include "Bisa/Features/Feature.h"

namespace Bisa {

    // TODO: Replace iterator with cutsom iterator
    // TODO: Combine with HitCollection (maybe)
    class BISA_API FeatureCollection
    {
    public:
        FeatureCollection()
        {
        }

        virtual ~FeatureCollection()
        {
        }

        void add(Ref<Feature> feature)
        {
            auto found = features_.find(feature->uniqueId());
            if (found == features_.end()) // if not found, add it
            {
                features_.insert({feature->uniqueId(), feature});
            }
        }

        void remove(Ref<Feature> feature)
        {
        }

        unsigned size() { return features_.size(); }

        std::unordered_map<unsigned int, Ref<Feature>>::iterator begin() { return features_.begin(); }
        std::unordered_map<unsigned int, Ref<Feature>>::iterator end() { return features_.end(); }

        std::unordered_map<unsigned int, Ref<Feature>>::const_iterator begin() const { return features_.begin(); }
        std::unordered_map<unsigned int, Ref<Feature>>::const_iterator end() const { return features_.end(); }

    private:
        std::unordered_map<unsigned int, Ref<Feature>> features_;
    };

}

