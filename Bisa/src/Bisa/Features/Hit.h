#pragma once

#include "bapch.h"
#include "Bisa/Config.h"

namespace Bisa {

    class BISA_API Hit
    {
    public:
        // Default constructor
        Hit() { }

        // Constructor to initialize all variables
        Hit(unsigned int trigger_id, 
            unsigned int bcid_fpga,
            unsigned int felix_counter,
            unsigned int tdc,
            unsigned int channel,
            unsigned int width,
            unsigned int bcid_tdc,
            unsigned int fine_time)
         : trigger_id_(trigger_id)
         , bcid_fpga_(bcid_fpga)
         , felix_counter_(felix_counter)
         , tdc_(tdc)
         , channel_(channel)
         , width_(width)
         , bcid_tdc_(bcid_tdc)
         , fine_time_(fine_time)
        {
            strip_ = CHANNEL_STRIP_MAPPING[channel];
            layer_ = config.layer(tdc);
            orientation_ = config.orientation(tdc);
            coordinate_ = config.coordinate(tdc);
            chamber_ = config.chamber(tdc);
            time_ = BC_RESOLUTION_NS * bcid_tdc + FINE_TIME_RESOLUTION_NS * fine_time;
        }

        // Get trigger id
        unsigned int trigger_id() const    { return trigger_id_; }
        // Get bcid from FPGA
        unsigned int bcid_fpga() const     { return bcid_fpga_; }
        // Get FELIX counter
        unsigned int felix_counter() const { return felix_counter_; }
        // Get tdc
        unsigned int tdc() const           { return tdc_; }
        // Get TDC channel
        unsigned int channel() const       { return channel_; }
        // Get TDC strip
        unsigned int strip() const         { return strip_; }
        // Get width (time over threshold)
        unsigned int width() const         { return width_; }
        // Get bcid from TDC
        unsigned int bcid_tdc() const      { return bcid_tdc_; }
        // Get fine time from TDC
        unsigned int fine_time() const     { return fine_time_; }
        
        // - Extra getters

        // Get layer
        unsigned int layer() const         { return layer_; }
        // Get orientation
        unsigned int orientation() const   { return orientation_; }
        // Get coordinate
        Coordinate coordinate() const      { return coordinate_; }
        // Get chamber
        unsigned int chamber() const       { return chamber_; }
        // Get time
        double time() const                { return time_; }

        
        // Set trigger id
        void set_trigger_id(unsigned int trigger_id)       { trigger_id_ = trigger_id; }
        // Set bcid from FPGA
        void set_bcid_fpga(unsigned int bcid_fpga)         { bcid_fpga_ = bcid_fpga; }
        // Set FELIX counter
        void set_felix_counter(unsigned int felix_counter) { felix_counter_ = felix_counter; }
        // Set tdc
        void set_tdc(unsigned int tdc)
        {
            tdc_ = tdc;
            layer_ = config.layer(tdc);
            orientation_ = config.orientation(tdc);
            coordinate_ = config.coordinate(tdc);
            chamber_ = config.chamber(tdc);
        }
        // Set TDC channel and strip
        void set_channel(unsigned int channel)
        { 
            channel_ = channel; 
            strip_ = CHANNEL_STRIP_MAPPING[channel]; 
        }
        // Set width (time over threshold)
        void set_width(unsigned int width)                 { width_ = width; }
        // Set bcid from TDC
        void set_bcid_tdc(unsigned int bcid_tdc)
        { 
            bcid_tdc_ = bcid_tdc; 
            time_ = BC_RESOLUTION_NS * bcid_tdc + FINE_TIME_RESOLUTION_NS * fine_time_;
        }
        // Set fine time from TDC
        void set_fine_time(unsigned int fine_time)         
        { 
            fine_time_ = fine_time; 
            time_ = BC_RESOLUTION_NS * bcid_tdc_ + FINE_TIME_RESOLUTION_NS * fine_time_;
        }

        // Return details of Hit as a string (debug tool)
        std::string to_string() const
        {
            std::stringstream ret;
            ret << ", Trigger ID: " << trigger_id_;
            ret << ", TDC: " << tdc_;
            ret << ", Channel: " << channel_;
            ret << ", Strip: " << strip_;
            ret << ", BCID (TDC): " << bcid_tdc_;
            ret << ", Fine Time: " << fine_time_;
            return ret.str();
        }

        // Compare if two hits are the same
        bool operator==(const Hit& other) const
        {
            if (trigger_id_ == other.trigger_id_ &&
                bcid_fpga_ == other.bcid_fpga_ &&
                felix_counter_ == other.felix_counter_ &&
                tdc_ == other.tdc_ &&
                channel_ == other.channel_ &&
                width_ == other.width_ &&
                bcid_tdc_ == other.bcid_tdc_ &&
                fine_time_ == other.fine_time_)
                return true;
            return false;
        }
        // Compare if two hits are different
        bool operator!=(const Hit& other) const
        {
            return !operator==(other);
        }

        // set comparator < for hits
        bool operator<(const Hit& other) const
        {
            // if (bcid_fpga_ < other.bcid_fpga_) return true;
            // if (felix_counter_ < other.felix_counter_) return true;
            if (trigger_id_ == other.trigger_id_)
            {
                if (tdc_ == other.tdc_)
                {
                    if (channel_ == other.channel_)
                    {
                        if (bcid_tdc_ == other.bcid_tdc_)
                        {
                            if (fine_time_ == other.fine_time_)
                            {
                                if (width_ == other.width_) return false;
                                return width_ < other.width_;
                            }
                            return fine_time_ < other.fine_time_;
                        }
                        return bcid_tdc_ < other.bcid_tdc_;
                    }
                    return channel_ < other.channel_;
                }
                return tdc_ < other.tdc_;
            }
            return trigger_id_ < other.trigger_id_;
        }

    private:
        unsigned int trigger_id_;
        unsigned int bcid_fpga_;
        unsigned int felix_counter_;

        unsigned int tdc_;
        unsigned int channel_;
        unsigned int width_;
        unsigned int bcid_tdc_;
        unsigned int fine_time_;

        // Extra hit data
        unsigned int strip_;
        unsigned int layer_;
        unsigned int orientation_;
        Coordinate coordinate_;
        unsigned int chamber_;
        double time_;
    };

}