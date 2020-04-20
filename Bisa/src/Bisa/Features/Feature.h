#pragma once

#include "bapch.h"

#include "HitCollection.h"

namespace Bisa {
    
    class Feature
    {
    public:
        Feature()
         : uniqueId_(uniqueIdCounter++)
        {
        }

        virtual ~Feature()
        {
        }

        Feature(const HitCollection& hits)
         : hits_(hits)
        {
        }

        HitCollection& hits()
        {
            return hits_;
        }

        unsigned int uniqueId() { return uniqueId_; }

    protected:
        static unsigned int uniqueIdCounter;

    private:
        HitCollection hits_;
        unsigned int uniqueId_;

    };

}