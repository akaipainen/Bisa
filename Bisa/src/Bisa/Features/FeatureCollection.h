#pragma once

#include "bapch.h"

#include "Bisa/Features/HitCollection.h"
#include "Bisa/Features/Feature.h"

namespace Bisa {

    class BISA_API FeatureCollection
    {
    public:
        class Iterator : public ::std::list<Ref<Feature>>::iterator
        {
        protected:
            using parent = ::std::list<Ref<Feature>>::iterator;

        public:
            Iterator(parent it)
             : parent(::std::move(it)) { }

            Feature& operator*() { return *(parent::operator*()); }

            Ref<Feature> operator->() { return *(parent::operator->()); }

            Ref<Feature> get() { return *(parent::operator->()); }
        };

        class ConstIterator : public ::std::list<Ref<Feature>>::const_iterator
        {
        protected:
            using parent = ::std::list<Ref<Feature>>::const_iterator;

        public:
            ConstIterator(parent it)
             : parent(::std::move(it)) { }

            Feature& operator*() { return *(parent::operator*()); }

            Ref<Feature> operator->() { return *(parent::operator->()); }

            Ref<Feature> get() { return *(parent::operator->()); }
        };

    public:
        // Default constructor
        FeatureCollection() = default;

        // Add a single feature to this collection
        void add(Ref<Feature> feature);

        // Remove a single feature from this collection
        void remove(Iterator feature_iter);

        // Remove a single feature from this collection
        void remove(ConstIterator feature_iter);

        // Remove a single feature from this collection
        void remove(Ref<Feature> feature);

        // Return the number of features in this collection
        unsigned int size() const { return features_.size(); }
        
        // Add a collection of features to this collection
        void add(const FeatureCollection& features);

        // Remove a collection of features from this collection
        void remove(const FeatureCollection& features);

        // Return a HitCollection of all hits in this feature collection (non-const)
        HitCollection hits();

        // Return a new feature collection consisting of features that
        // are the pairwise set intersection of hits in each of the previous 
        // collections (maybe not needed)
        FeatureCollection operator&(const FeatureCollection& other) const;

        // Return a new feature collection consisting of features that
        // are the pairwise set union of hits in each of the previous 
        // collections (maybe not needed)
        // FeatureCollection operator|(const FeatureCollection& other) const;

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
        ::std::list<Ref<Feature>> features_;
        ::std::unordered_set<unsigned int> feature_ids_;

        unsigned int trigger_id_;
    };
}
