#pragma once

#include "bapch.h"

#include "HitCollection.h"

namespace Bisa {

    // Interface to inherit from when constructing Features
    class BISA_API HitSelector
    {
    public:
        using CreateHitCallbackFn = std::function<void(Feature&)>;

        HitSelector()
        {
        }

        virtual ~HitSelector()
        {
        }

        void setCreateHitCallback(const CreateHitCallbackFn& callback) { createHitCallback_ = callback; }

        virtual HitCollection operator()(const HitCollection& filterHits) const = 0;

    protected:
        CreateHitCallbackFn createHitCallback_;
    };

}