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
        Config() {}

        bool pairmode() const { return data_.at("pairmode").get<bool>(); }
        double run_duration_s() const { return data_.at("run_duration_s").get<unsigned int>(); }
        bool internal_autotrigger_enabled() const { return data_.at("internal_autotrigger").get<bool>(); }

        json exparam() const { return data_["experiments"]; }

        int layer(unsigned int tdc) const { return tdc_layer_mapping_[tdc]; }
        int orientation(unsigned int tdc) const { return tdc_orientation_mapping_[tdc]; }
        Coordinate coordinate(unsigned int tdc) const { return tdc_coordinate_mapping_[tdc]; }
        int chamber(unsigned int tdc) const { return tdc_chamber_mapping_[tdc]; }
        
        void init(const std::string &config_filename)
        {
            // Open config file and parse json data
            std::fstream file;
            file.open(config_filename);
            BA_CORE_ASSERT(file.is_open(), "Unable to open config file");
            file >> data_;

            // Initialize chamber tdc mapping
            int counter = 0;
            for (auto &&tdc : data_.at("bis7").at("eta_layer_0_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 0;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis7").at("eta_layer_1_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 1;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis7").at("eta_layer_2_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 2;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis7").at("phi_layer_0_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 0;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis7").at("phi_layer_1_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 1;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis7").at("phi_layer_2_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 2;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 7;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis8").at("eta_layer_0_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 0;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis8").at("eta_layer_1_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 1;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis8").at("eta_layer_2_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 2;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = ETA;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis8").at("phi_layer_0_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 0;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis8").at("phi_layer_1_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 1;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 8;
            }

            counter = 0;
            for (auto &&tdc : data_.at("bis8").at("phi_layer_2_tdcs").get<std::vector<unsigned int>>())
            {
                tdc_layer_mapping_[tdc] = 2;
                tdc_orientation_mapping_[tdc] = counter++;
                tdc_coordinate_mapping_[tdc] = PHI;
                tdc_chamber_mapping_[tdc] = 8;
            }

            for (auto &&tdc : data_.at("scintillator").at("tdcs").get<std::vector<unsigned int>>())
            {
                tdc_chamber_mapping_[tdc] = 1;
            }


            BA_CORE_INFO("Initialized config");
            BA_CORE_INFO("Trigger: {}", internal_autotrigger_enabled() ? "Enabled" : "Disabled");
        }

    private:
        json data_;

        unsigned int tdc_layer_mapping_[18]; // 0, 1, 2
        unsigned int tdc_orientation_mapping_[18]; // LEFT/RIGHT
        Coordinate tdc_coordinate_mapping_[18]; // ETA/PHI
        unsigned int tdc_chamber_mapping_[18]; // 0/1/7/8 (0=off, 1=scintillator, 7=bis7, 8=bis8)
    };

    extern Config config; // global config
    
}

