#pragma once

#include "bapch.h"

#include "Bisa/Features/HitCollection.h"

namespace Bisa {

    class BISA_API FeatureCollection
    {
    public:

        class Iterator : public ::std::vector<HitCollection>::iterator {
        protected:
            using parent = ::std::vector<HitCollection>::iterator;

        public:
            Iterator(parent it)
             : parent(::std::move(it)) { }
        };

        class ConstIterator : public ::std::vector<HitCollection>::const_iterator
        {
        protected:
            using parent = ::std::vector<HitCollection>::const_iterator;

        public:
            ConstIterator(parent it)
             : parent(::std::move(it)) { }
        };

    public:
        // Default constructor
        FeatureCollection() = default;

        // Add a single feature to this collection
        void add(const HitCollection& feature);

        // Return the number of features in this collection
        unsigned int size() const { return features_.size(); }

        // Return a HitCollection of all hits in this feature collection
        HitCollection hits();

        // Return string interpretation of this object (debug tool)
        ::std::string to_string() const;

        // Return begin iterator (non-const)
        Iterator begin() { return features_.begin(); }
        
        // Return end iterator (non-const)
        Iterator end() { return features_.end(); }

        // Return begin iterator (const)
        ConstIterator begin() const { return features_.begin(); }
        
        // Return end iterator (const)
        ConstIterator end() const { return features_.end(); }

    private:
        ::std::vector<HitCollection> features_;

        unsigned int trigger_id_;
    };
}
