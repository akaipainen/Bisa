#pragma once

#include "bapch.h"

namespace Bisa {

    class BISA_API Hit
    {
    public:
        // Default constructor
        Hit()
         : unique_id_(s_unique_id_counter_++)
        {
        }

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
         , unique_id_(s_unique_id_counter_++)
        {
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
        // Get width (time over threshold)
        unsigned int width() const         { return width_; }
        // Get bcid from TDC
        unsigned int bcid_tdc() const      { return bcid_tdc_; }
        // Get fine time from TDC
        unsigned int fine_time() const     { return fine_time_; }
        // Get Hit unique id
        unsigned int unique_id() const     { return unique_id_; }
        
        // Set trigger id
        void set_trigger_id(unsigned int trigger_id)       { trigger_id_ = trigger_id; }
        // Set bcid from FPGA
        void set_bcid_fpga(unsigned int bcid_fpga)         { bcid_fpga_ = bcid_fpga; }
        // Set FELIX counter
        void set_felix_counter(unsigned int felix_counter) { felix_counter_ = felix_counter; }
        // Set tdc
        void set_tdc(unsigned int tdc)                     { tdc_ = tdc; }
        // Set TDC channel
        void set_channel(unsigned int channel)             { channel_ = channel; }
        // Set width (time over threshold)
        void set_width(unsigned int width)                 { width_ = width; }
        // Set bcid from TDC
        void set_bcid_tdc(unsigned int bcid_tdc)           { bcid_tdc_ = bcid_tdc; }
        // Set fine time from TDC
        void set_fine_time(unsigned int fine_time)         { fine_time_ = fine_time; }

        // Return details of Hit as a string (debug tool)
        std::string to_string()
        {
            std::stringstream ret;
            ret << "Unique ID: " << unique_id_;
            ret << ", Trigger ID: " << trigger_id_;
            ret << ", TDC: " << tdc_;
            ret << ", Channel: " << channel_;
            ret << ", BCID (TDC): " << bcid_tdc_;
            ret << ", Fine Time: " << fine_time_;
            return ret.str();
        }

        // Compare if two hits are the same
        bool operator==(const Hit& other)
        {
            return unique_id_ == other.unique_id_;
        }
        // Compare if two hits are different
        bool operator!=(const Hit& other)
        {
            return !operator==(other);
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

        unsigned int unique_id_;

        static unsigned int s_unique_id_counter_;
    };

}