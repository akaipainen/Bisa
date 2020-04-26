#pragma once

#include "bapch.h"

#include "Bisa/Core.h"
#include "Bisa/Features/Hit.h"

namespace Bisa {

    class BISA_API HitCollection
    {
    public:
        // Custom iterators to make interface with HitCollection much nicer
        // ConstIterator is const version of Iterator,
        // both are derived classes of list::(const_)iterator

        class Iterator : public ::std::list<Ref<Hit>>::iterator
        {
        protected:
            using parent = ::std::list<Ref<Hit>>::iterator;

        public:
            Iterator(parent it)
             : parent(std::move(it)) { }

            Hit& operator*() { return *(parent::operator*()); }

            Ref<Hit> operator->() { return *(parent::operator->()); }

            Ref<Hit> get() { return *(parent::operator->()); }
        };

        class ConstIterator : public ::std::list<Ref<Hit>>::const_iterator
        {
        protected:
            using parent = std::list<Ref<Hit>>::const_iterator;

        public:
            ConstIterator(parent it)
             : parent(std::move(it)) { }

            Hit& operator*() { return *(parent::operator*()); }

            Ref<Hit> operator->() { return *(parent::operator->()); }

            Ref<Hit> get() { return *(parent::operator->()); }
        };

    public:
        // Default constructor
        HitCollection() = default;
        HitCollection(const HitCollection& other) = default;

        // Add a single hit to this collection
        void add(Ref<Hit> hit);

        // Remove a single feature from this collection
        void remove(Iterator hit_iter);

        // Remove a single feature from this collection
        // ConstIterator erase not working on GCC 4.8.5, was fixed in 4.9.0
        // void remove(ConstIterator hit_iter);

        // Remove a single feature from this collection
        void remove(Ref<Hit> hit);

        // Get the trigger id of this collection 
        unsigned int trigger_id() const { return trigger_id_; }

        // Return the number of hits in this collection
        unsigned int size() const { return hits_.size(); }
        
        // Add a collection of hits to this collection
        void add(const HitCollection& hits);

        // Remove a collection of hits from this collection
        void remove(const HitCollection& hits);

        // Return set intersection of this collection with another
        HitCollection operator&(const HitCollection& other) const;

        // Return set union of this collection with another
        HitCollection operator|(const HitCollection& other) const;

        // Return set difference of this collection with another
        HitCollection operator-(const HitCollection& other) const;

        // Return string interpretation of this object (debug tool)
        ::std::string to_string() const;

        // Return begin iterator (non-const)
        Iterator begin() { return hits_.begin(); }
        
        // Return end iterator (non-const)
        Iterator end() { return hits_.end(); }

        // Return begin iterator (const)
        ConstIterator begin() const { return hits_.begin(); }
        
        // Return end iterator (const)
        ConstIterator end() const { return hits_.end(); }

    private:
        ::std::list<Ref<Hit>> hits_;
        ::std::unordered_set<unsigned int> hit_ids_;

        unsigned int trigger_id_;
    };

}
