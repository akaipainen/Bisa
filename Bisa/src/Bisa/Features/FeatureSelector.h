#pragma once

#include "bapch.h"

#include "Bisa/Config.h"
#include "Feature.h"
#include "HitCollection.h"
#include "FeatureCollection.h"

namespace Bisa {

    // Interface to inherit from when constructing Features
    class BISA_API FeatureSelector
    {
    public:
        using CreateFeatureCallbackFn = ::std::function<void(Feature&)>;

        FeatureSelector(const Config& config)
         : config_(config)
        {
        }

        virtual ~FeatureSelector()
        {
        }

        // void setCreateFeatureCallback(const CreateFeatureCallbackFn& callback) { createFeatureCallback_ = callback; }

        virtual FeatureCollection operator()(const HitCollection& filterHits) const = 0;

    protected:
        const Config& config_;
        // CreateFeatureCallbackFn createFeatureCallback_;
    };

}