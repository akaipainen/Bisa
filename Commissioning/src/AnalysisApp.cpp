#include <Bisa.h>

#include "Features/SpaceCluster.h"
#include "Features/NoiseBurst.h"
#include "Features/TimeCluster.h"
#include "Features/FirstHitOnStrip.h"
#include "Features/SelectHits.h"
#include "Features/AdjacentHits.h"
#include "Features/MuonCandidate.h"
#include "Features/MuonCandidateNoTime.h"

#include "Plots/EventSummary.h"
#include "Plots/HitDistribution.h"
#include "Plots/RandomNoiseDistribution.h"
#include "Plots/FirstHitsDistribution.h"
#include "Plots/MuonHitsDistribution.h"
#include "Plots/NoiseBurstDistribution.h"
#include "Plots/WidthDistribution.h"
#include "Plots/TimingResolution.h"
#include "Plots/AdjacentHitsDistribution.h"
#include "Plots/HitPairsTimeDistance.h"
#include "Plots/FEClusterSize.h"

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp(const Bisa::Config& config)
    : Bisa::Application(config)
    , all_first_hd_("all_first_hits", config)
    , select_hd_("select_spike_hits", config)
    , muon_hd_("muon_hits", config)
    , random_noise_hd_("random_noise_hits", config)
    , noise_burst_hd_("noise_burst_hits", config)
    , all_first_wd_("all_first_width", config)
    , muon_wd_("muon_width", config)
    , random_noise_wd_("random_noise_width", config)
    , noise_burst_wd_("noise_burst_width", config)
    , timing_resolution_("timing_resolution", config)
    , adjacent_hits_left_("adjacent_hits_left", config)
    , adjacent_hits_right_("adjacent_hits_right", config)
    , cable_mapping_("cable_mapping", config)
    , fe_cluster_size_("fec_cluster_size", config)
    , firstHitSelector(config)
    , selectHitsSelector(config)
    , spaceClusterSelector(config)
    , noiseBurstSelector(config)
    , timeClusterSelector(config)
    , adjacentHitsSelector(config)
    , muonCandidateSelector(config)
    , muonCandidateNoTimeSelector(config)
    {
    }

    ~AnalysisApp()
    {
    }

    void Run() override
    {
        //  TODO: Remove
        gDirectory->mkdir("event_display");
        while (dataStream_->fill_next_event())
        {        
            Step();
        }
        printf("Event Counter: %d\n"
                "Muon Event Counter: %d\n"
                "\tClean Muon Event Counter: %d\n"
                "Noise Burst Event Counter: %d\n"
                "\tSingle Layer: %d\n"
                "\tMultiple Layer: %d\n",
                event_counter_,
                muon_event_counter_,
                clean_muon_event_counter_,
                noise_burst_event_counter_,
                single_layer_noise_burst_event_counter_,
                multiple_layer_noise_burst_event_counter_);
        printf("Muon hits on each layer:\n"
                "\tEta:\n"
                "\t\tLayer 1-2: %d\n"
                "\t\tLayer 1-3: %d\n"
                "\t\tLayer 2-3: %d\n"
                "\t\tLayer 1-2-3: %d\n"
                "\tPhi:\n"
                "\t\tLayer 1-2: %d\n"
                "\t\tLayer 1-3: %d\n"
                "\t\tLayer 2-3: %d\n"
                "\t\tLayer 1-2-3: %d\n",
                muon_eta_layers_hit_[3],
                muon_eta_layers_hit_[4],
                muon_eta_layers_hit_[5],
                muon_eta_layers_hit_[6],
                muon_phi_layers_hit_[3],
                muon_phi_layers_hit_[4],
                muon_phi_layers_hit_[5],
                muon_phi_layers_hit_[6]);
        printf("Efficiency:%f\n", float(efficiency_num_) / float(efficiency_denom_));
        file_->Write();
    }

    void Step() override
    {

        Bisa::HitCollection firstHits = firstHitSelector(*hits_);

        Bisa::FeatureCollection noiseBursts = noiseBurstSelector(*hits_);

        Bisa::FeatureCollection spaceClusters = spaceClusterSelector(firstHits);
        Bisa::FeatureCollection timeClusters = timeClusterSelector(firstHits);
        Bisa::HitCollection randomNoise = (firstHits - timeClusters.hits()) - spaceClusters.hits();

        Bisa::FeatureCollection muonCandidates = muonCandidateSelector(firstHits);
        Bisa::FeatureCollection muonSpaceCandidates = muonCandidateNoTimeSelector(firstHits);

        Bisa::HitCollection filteredHits = muonCandidates.hits() - noiseBursts.hits();
        Bisa::FeatureCollection leftAdjacentHits = adjacentHitsSelector(filteredHits, -1);
        Bisa::FeatureCollection rightAdjacentHits = adjacentHitsSelector(filteredHits, 1);
        
        std::unordered_map<int, int> filtered_hits_on_tdc_;
        for (int i = 0; i < 18; i++)
        {
            filtered_hits_on_tdc_[i] = 0;
        }
        
        for (auto &&hit : filteredHits)
        {
            filtered_hits_on_tdc_[hit.tdc()]++;
        }

        // if (noiseBursts.hits().size() > 0)
        // {
        //     if (muonCandidates.hits().size() > 0)
        //     {
        //         EventSummary es1("events_muon_incidence", config_);
        //         es1.configureAllHits(*hits_);
        //         es1.addHits(*hits_, kRed);
        //         es1.addHits(muonCandidates.hits(), kBlue);
        //     }
        //     else
        //     {
        //         EventSummary es1("events_no_muons", config_);
        //         es1.configureAllHits(*hits_);
        //         es1.addHits(*hits_, kRed);
        //     }
        // }

        // if (muon_event_counter_ < 100)
        // { 
        //     EventSummary es4("events_muon_candidates", config_);
        //     es4.configureAllHits(*hits_);
        //     es4.addHits(*hits_, kBlack);
        //     es4.addHits(muonCandidates.hits(), kRed);
        //     muon_event_counter_++;
        // }

        // for (auto &&h : leftAdjacentHits.hits())
        // {
        //     if (filtered_hits_on_tdc_[h.tdc()] <= 3)
        //     {
        //         if (h.tdc() == 4 && config_.strip(h.channel()) == 17)
        //         {
        //             // BA_INFO("Event ID: {} | Num Hits: {}", h.trigger_id(), hits_->size());

        //             EventSummary es4("events_adjacent_left_t4s17", config_);
        //             es4.configureAllHits(*hits_);
        //             es4.addHits(*hits_, kRed);
        //             es4.addHits(muonCandidates.hits() - noiseBursts.hits(), kBlue);
        //             // es4.addHits(firstHits - noiseBursts.hits(), kRed);
        //             break;
        //         }
        //     }
        // }

        Bisa::HitCollection selectSpikeHits = selectHitsSelector(firstHits, 4, 17);

        all_first_hd_.addHits(firstHits);
        if (selectSpikeHits.size() > 0)
        {
            select_hd_.addHits(firstHits);
        }
        muon_hd_.addHits(muonCandidates.hits());
        random_noise_hd_.addHits(randomNoise);
        noise_burst_hd_.addHits(noiseBursts.hits());

        all_first_wd_.addHits(firstHits);
        muon_wd_.addHits(muonCandidates.hits());
        random_noise_wd_.addHits(randomNoise);
        noise_burst_wd_.addHits(noiseBursts.hits());

        for (auto &&f : spaceClusters)
        {
            fe_cluster_size_.addHits(f.hits());
        }

        if (noiseBursts.size() == 0)
        {
            timing_resolution_.calculateMeanTime(muonCandidates.hits());
            timing_resolution_.fillHits(muonSpaceCandidates.hits());
        }

        for (auto &&hit : leftAdjacentHits.hits())
        {
            if (filtered_hits_on_tdc_[hit.tdc()] <= 2)
            {
                adjacent_hits_left_.addHits(leftAdjacentHits.hits());
            }
        }
        for (auto &&hit : rightAdjacentHits.hits())
        {
            if (filtered_hits_on_tdc_[hit.tdc()] <= 2)
            {
                adjacent_hits_right_.addHits(rightAdjacentHits.hits());
            }
        }

        // if (leftAdjacentHits.hits().size() == 2)
        // {
        //     adjacent_hits_left_.addHits(leftAdjacentHits.hits());
        // }
        // if (rightAdjacentHits.hits().size() == 2)
        // {
        //     adjacent_hits_right_.addHits(rightAdjacentHits.hits());
        // }

        cable_mapping_.addHits(firstHits);

        // Efficiency calculation
        unsigned int fixed1 = 0;
        unsigned int fixed2 = 1;
        unsigned int changing = 2;
        if (noiseBursts.hits().size() == 0)
        {
            for (auto &&hit1 : muonCandidates.hits())
            {
                if (config_.chamber(hit1.tdc()) == 7)
                {
                    fixed1 = 0;
                    fixed2 = 2;
                    changing = 1;
                }
                else
                {
                    fixed1 = 0;
                    fixed2 = 1;
                    changing = 2;
                }
                if (config_.layer(hit1.tdc()) == fixed1)
                {
                    for (auto &&hit2 : muonCandidates.hits())
                    {
                        if (config_.layer(hit2.tdc()) == fixed2)
                        {
                            if (std::abs(config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel()) - 
                                        config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel())) < 3 &&
                                config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()))
                            {
                                efficiency_denom_++;

                                // Check for 3rd hit
                                for (auto &&hit3 : muonCandidates.hits())
                                {
                                    if (config_.chamber(hit3.tdc()) == config_.chamber(hit1.tdc()))
                                    {
                                        if (config_.layer(hit3.tdc()) == changing)
                                        {
                                            if (std::abs(config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel()) - 
                                                        config_.orientation(hit3.tdc())*32+config_.strip(hit3.channel())) < 3 &&
                                                std::abs(config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel()) - 
                                                        config_.orientation(hit3.tdc())*32+config_.strip(hit3.channel())) < 3)
                                            {
                                                efficiency_num_++;
                                                break;
                                            }
                                        }
                                    }
                                }
                                
                            }
                        }
                    }
                }            
            }           
        }

        // Update counters
        event_counter_++;
        if (muonCandidates.size() > 0)
        {
            muon_event_counter_++;
            if (noiseBursts.hits().size() == 0)
            {
                clean_muon_event_counter_++;
            }

            unsigned int muon_eta_layer_sum_ = 0;
            unsigned int muon_phi_layer_sum_ = 0;
            for (auto &&hit : muonCandidates.hits())
            {
                if (config_.coordinate(hit.tdc()) == Bisa::ETA)
                {
                    unsigned int layer = config_.layer(hit.tdc()) + 1;
                    if (muon_eta_layers_.find(layer) == muon_eta_layers_.end())
                    {
                        muon_eta_layer_sum_ += layer;
                        muon_eta_layers_.insert(layer);
                    }
                }
                else
                {
                    unsigned int layer = config_.layer(hit.tdc()) + 1;
                    if (muon_phi_layers_.find(layer) == muon_phi_layers_.end())
                    {
                        muon_phi_layer_sum_ += layer;
                        muon_phi_layers_.insert(layer);
                    }
                }
            }
            muon_phi_layers_.clear();
            muon_eta_layers_.clear();
            muon_eta_layers_hit_[muon_eta_layer_sum_] += 1;
            muon_phi_layers_hit_[muon_phi_layer_sum_] += 1;

        }
        if (noiseBursts.size() > 0)
        {
            noise_burst_event_counter_++;
            auto feature_it = noiseBursts.begin();
            int layer = config_.layer(feature_it->hits().begin()->tdc());
            for (; feature_it != noiseBursts.end(); feature_it++)
            {
                if (config_.layer(feature_it->hits().begin()->tdc()) != layer)
                {
                    multiple_layer_noise_burst_event_counter_++;
                    break;
                }
            }
            if (feature_it == noiseBursts.end())
            {
                single_layer_noise_burst_event_counter_++;
            }
        }
    }

private:
    FirstHitsDistribution all_first_hd_;
    FirstHitsDistribution select_hd_;
    MuonHitsDistribution muon_hd_;
    RandomNoiseDistribution random_noise_hd_;
    NoiseBurstDistribution noise_burst_hd_;

    WidthDistribution all_first_wd_;
    WidthDistribution muon_wd_;
    WidthDistribution random_noise_wd_;
    WidthDistribution noise_burst_wd_;

    TimingResolution timing_resolution_;

    AdjacentHitsDistribution adjacent_hits_left_;
    AdjacentHitsDistribution adjacent_hits_right_;

    HitPairsTimeDistance cable_mapping_;

    FEClusterSize fe_cluster_size_;

    FirstHitOnStripSelector firstHitSelector;
    SelectHitsSelector selectHitsSelector;
    SpaceClusterSelector spaceClusterSelector;
    NoiseBurstSelector noiseBurstSelector;
    TimeClusterSelector timeClusterSelector;
    AdjacentHitsSelector adjacentHitsSelector;
    MuonCandidateSelector muonCandidateSelector;
    MuonCandidateNoTimeSelector muonCandidateNoTimeSelector;

    unsigned int noise_burst_event_counter_ = 0;
    unsigned int single_layer_noise_burst_event_counter_ = 0;
    unsigned int multiple_layer_noise_burst_event_counter_ = 0;
    unsigned int efficiency_num_ = 0;
    unsigned int efficiency_denom_ = 0;
    unsigned int muon_event_counter_ = 0;
    std::unordered_set<unsigned int> muon_eta_layers_;
    std::unordered_set<unsigned int> muon_phi_layers_;
    std::vector<unsigned int> muon_eta_layers_hit_ = std::vector<unsigned int>(7, 0);
    std::vector<unsigned int> muon_phi_layers_hit_ = std::vector<unsigned int>(7, 0);
    unsigned int clean_muon_event_counter_ = 0;
    unsigned int event_counter_ = 0;
};

Bisa::Application* Bisa::CreateApplication(const Bisa::Config& config)
{
    return new AnalysisApp(config);
}
