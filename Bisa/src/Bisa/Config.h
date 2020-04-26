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
            tdc_layer_mapping_.reserve(18);
            tdc_orientation_mapping_.reserve(18);
            tdc_coordinate_mapping_.reserve(18);
            tdc_chamber_mapping_.reserve(18);

            // Initialize values from config file
            init();

            BA_CORE_INFO("Initialized config");
        }

        std::string get_path_to_data() const { return filename_; }
        bool get_pairmode() const { return pairmode_; }
        
    private:
        void init()
        {
            // Initialize data properties
            config_data_.at("path_to_data").get_to(filename_);
            config_data_.at("pairmode").get_to(pairmode_);

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

        std::vector<unsigned int> tdc_layer_mapping_; // 0, 1, 2
        std::vector<unsigned int> tdc_orientation_mapping_; // LEFT/RIGHT
        std::vector<Coordinate> tdc_coordinate_mapping_; // ETA/PHI
        std::vector<unsigned int> tdc_chamber_mapping_; // 7/8
        
        std::vector<unsigned int> strip_mapping_; // Channel -> Strip
    };

    extern Config& config;
    
}

