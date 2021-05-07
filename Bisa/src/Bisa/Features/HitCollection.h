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

        typedef std::set<Hit>::iterator Iterator;
        typedef std::set<Hit>::const_iterator ConstIterator;

        // class Iterator : public ::std::vector<Hit>::iterator
        // {
        // protected:
        //     using parent = ::std::vector<Hit>::iterator;

        // public:
        //     Iterator(parent it)
        //      : parent(std::move(it)) { }
        // };

        // class ConstIterator : public ::std::vector<Hit>::const_iterator
        // {
        // protected:
        //     using parent = std::vector<Hit>::const_iterator;

        // public:
        //     ConstIterator(parent it)
        //      : parent(std::move(it)) { }
        // };

    public:
        // Default constructor
        HitCollection() = default;
        HitCollection(const HitCollection& other) = default;

        // Add a single hit to this collection
        void add(const Hit& hit);

        // Clear the entire hit collection
        void clear();

        // Get the trigger id of this collection 
        unsigned int trigger_id() const { return trigger_id_; }

        // Set the trigger id of this collection 
        void set_trigger_id(const unsigned int id)
        { 
            trigger_id_ = id;
            auto temp = hits_;
            hits_.clear();
            for (auto hit : temp)
            {
                hit.set_trigger_id(trigger_id_);
                hits_.insert(hit);
            }
        }

        // Return the number of hits in this collection
        unsigned int size() const { return hits_.size(); }

        // Return whether the collection is empty
        bool empty() const { return hits_.size() == 0; }

        // Return set intersection of this collection with another
        HitCollection operator&(const HitCollection& other) const;

        // Return set union of this collection with another
        HitCollection operator|(const HitCollection& other) const;

        // Return set difference of this collection with another
        HitCollection operator-(const HitCollection& other) const;

        // Return string interpretation of this object (debug tool)
        ::std::string to_string() const;

        // Return begin iterator (const)
        Iterator begin() { return hits_.begin(); }
        
        // Return end iterator (const)
        Iterator end() { return hits_.end(); }

        // Return begin iterator (const)
        ConstIterator begin() const { return hits_.begin(); }
        
        // Return end iterator (const)
        ConstIterator end() const { return hits_.end(); }

        // Get the underlying vector address
        ::std::set<Hit> * get_hits_address() { return &hits_; }

    private:
        ::std::set<Hit> hits_;
        unsigned int trigger_id_ = 0;
    };

}
