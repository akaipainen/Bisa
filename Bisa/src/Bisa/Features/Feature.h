#pragma once

#include "bapch.h"

#include "HitCollection.h"

namespace Bisa {
    
    class BISA_API Feature
    {
    public:
        // Default constructor
        Feature()
         : unique_id_(s_unique_id_counter++)
         , trigger_id_(0)
        {
        }

        // Constructor from existing hit collection
        Feature(const HitCollection& hits)
         : hits_(hits)
         , unique_id_(s_unique_id_counter++)
         , trigger_id_(hits.trigger_id())
        {
        }

        // Return hits by reference (non-const)
        HitCollection& hits()
        {
            return hits_;
        }

        unsigned int size()
        {
            return hits_.size();
        }

        // Get unique id of this feature
        unsigned int unique_id() { return unique_id_; }
        
        // Get trigger id of this feature
        unsigned int trigger_id() { return trigger_id_; }

        // Compare if two features are the same
        bool operator==(const Feature& other)
        {
            return unique_id_ == other.unique_id_;
        }
        // Compare if two features are different
        bool operator!=(const Feature& other)
        {
            return !operator==(other);
        }

    protected:
        static unsigned int s_unique_id_counter;

    private:
        HitCollection hits_;
        unsigned int unique_id_;
        unsigned int trigger_id_;

    };

}