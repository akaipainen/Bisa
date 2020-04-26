#pragma once
#include "bapch.h"

#include "nlohmann/json.hpp"

namespace Bisa {

    enum Coordinate {
        ETA,
        PHI
    };

    // For convenience
    using json = nlohmann::json;

    class Config
    {
    public:
        Config(std::string config_filename)
        {
            // Open config file and parse json data
            std::fstream file;
            file.open(config_filename);
            BA_CORE_ASSERT(file.is_open(), "Unable to open config file");
            file >> config_data_;

            // Reserve vector capacity
            // std::fill(tdc_layer_mapping_, tdc_layer_mapping_+17, 0);
            // std::fill(tdc_orientation_mapping_, tdc_layer_mapping_+17, 0);
            // std::fill(tdc_coordinate_mapping_, tdc_layer_mapping_+17, 0);
            // std::fill(tdc_chamber_mapping_, tdc_layer_mapping_+17, 0);

            // Initialize values from config file
            init();

            BA_CORE_INFO("Initialized config");
        }

        std::string path_to_data() const { return filename_; }
        bool pairmode() const { return pairmode_; }
        double bcid_resolution() const { return bcid_resolution_; }
        double fine_time_resolution() const { return fine_time_resolution_; }
        double run_duration() const { return run_duration_; }

        int strip(unsigned int channel) const { return strip_mapping_[channel]; }
        
        int layer(unsigned int tdc) const { return tdc_layer_mapping_[tdc]; }
        int orientation(unsigned int tdc) const { return tdc_orientation_mapping_[tdc]; }
        int coordinate(unsigned int tdc) const { return tdc_coordinate_mapping_[tdc]; }
        int chamber(unsigned int tdc) const { return tdc_chamber_mapping_[tdc]; }
        
    private:
        void init()
        {
            // Initialize data properties
            config_data_.at("path_to_data").get_to(filename_);
            config_data_.at("pairmode").get_to(pairmode_);
            config_data_.at("run_duration_seconds").get_to(run_duration_);

            // Initialize TDC constants
            config_data_.at("tdc").at("bcid_resolution_ns").get_to(bcid_resolution_);
            config_data_.at("tdc").at("fine_time_resolution_ns").get_to(fine_time_resolution_);

            // Initialize channel to strip mapping
            config_data_.at("channel_to_strip_cable_mapping").get_to(strip_mapping_);

            // Initialize chamber geometry
            int counter = 0;
            for (auto &&tdc : config_data_.at("bis7").at("eta_layer_0_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 0;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis7").at("eta_layer_1_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 1;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis7").at("eta_layer_2_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 2;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis7").at("phi_layer_0_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 0;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis7").at("phi_layer_1_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 1;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis7").at("phi_layer_2_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 2;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis8").at("eta_layer_0_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 0;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis8").at("eta_layer_1_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 1;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis8").at("eta_layer_2_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 2;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis8").at("phi_layer_0_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 0;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis8").at("phi_layer_1_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 1;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : config_data_.at("bis8").at("phi_layer_2_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 2;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 8;
            }
        }

    private:
        json config_data_;

        // Parameters

        std::string filename_;
        bool pairmode_;
        double run_duration_;

        double bcid_resolution_;
        double fine_time_resolution_;

        unsigned int tdc_layer_mapping_[18]; // 0, 1, 2
        unsigned int tdc_orientation_mapping_[18]; // LEFT/RIGHT
        Coordinate tdc_coordinate_mapping_[18]; // ETA/PHI
        unsigned int tdc_chamber_mapping_[18]; // 7/8
        
        unsigned int strip_mapping_[32]; // Channel -> Strip
    };

    extern Config& config;
    
}

