#include <Bisa.h>
#include <Bisa/EntryPoint.h>

#include "FeatureSelector.h"
#include "SelectorFilter.h"
#include "HitSelector.h"

// #include "Features/SpaceCluster.h"
// #include "Features/LayerSpaceCluster.h"
// #include "Features/NoiseBurst.h"
// #include "Features/TimeCluster.h"
// #include "Features/FirstHitOnStrip.h"
// #include "Features/SelectHits.h"
// #include "Features/AdjacentHits.h"
// #include "Features/MuonCandidate.h"
// #include "Features/MuonCandidateNoTime.h"

#include "Plots/EventSummary.h"
#include "Plots/HitDistribution.h"
#include "Plots/RandomNoiseDistribution.h"
#include "Plots/FirstHitsDistribution.h"
#include "Plots/MuonHitsDistribution.h"
#include "Plots/NoiseBurstDistribution.h"
#include "Plots/WidthDistribution.h"
#include "Plots/TimingResolution.h"
#include "Plots/NumberOfHits.h"
#include "Plots/AdjacentHitsDistribution.h"
#include "Plots/HitPairsTimeDistance.h"
#include "Plots/FEClusterSize.h"
#include "Plots/ClusterSize.h"
#include "Plots/LatencyTDCFPGA.h"
#include "Plots/FPGABCIDDistribution.h"
#include "Plots/CrosstalkWidth.h"

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp(const Bisa::Config& config)
    : Bisa::Application(config)
    , featureSelector_(config)
    , hitSelector_(config)
    , all_first_hd_("all_first_hits", config)
    , select_hd_("select_spike_hits", config)
    , muon_hd_("muon_hits", config)
    , exlusive_muon_hd_("exlusive_muon_hits", config)
    , random_noise_hd_("random_noise_hits", config)
    , noise_burst_hd_("noise_burst_hits", config)
    , all_first_wd_("all_first_width", config)
    , muon_wd_("muon_width", config)
    , random_noise_wd_("random_noise_width", config)
    , noise_burst_wd_("noise_burst_width", config)
    , timing_resolution_("timing_resolution", config)
    , number_of_hits_("number_of_hits", config)
    , adjacent_hits_left_("adjacent_hits_left", config)
    , adjacent_hits_right_("adjacent_hits_right", config)
    , cable_mapping_("cable_mapping", config)
    , fe_cluster_size_("fec_cluster_size", config)
    , cluster_size_("cluster_size", config)
    , latency_("latency", config)
    , event_bcid_dist_("event_fpga_bcid", config)
    , crosstalk_wd_("crosstalk_width", config)
    // , firstHitSelector(config)
    // , selectHitsSelector(config)
    // , spaceClusterSelector(config)
    // , layerSpaceClusterSelector(config)
    // , noiseBurstSelector(config)
    // , timeClusterSelector(config)
    // , adjacentHitsSelector(config)
    // , muonCandidateSelector(config)
    // , muonCandidateNoTimeSelector(config)
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
        printf("Efficiency 0:%f\n", float(efficiency_num_0_) / float(efficiency_denom_0_));
        printf("Efficiency 1:%f\n", float(efficiency_num_1_) / float(efficiency_denom_1_));
        printf("Efficiency 2:%f\n", float(efficiency_num_2_) / float(efficiency_denom_2_));
        file_->Write();
    }

    void Step() override
    {
        Bisa::HitCollection firstHits = hits_;
        Bisa::FeatureCollection spaceClusters = featureSelector_.basicSelector(firstHits, SelectorFilter::adjacent_hits);
        Bisa::FeatureCollection noiseBursts = featureSelector_.requireMinSize(spaceClusters, 8);
        
        Bisa::FeatureCollection muonCandidates = featureSelector_.basicSelector(firstHits, SelectorFilter::muon_candidate);
        muonCandidates = featureSelector_.requireMultipleLayers(muonCandidates, 2);
        Bisa::FeatureCollection exlusiveMuonCandidates = featureSelector_.restrictMaxHitsPerLayer(muonCandidates, 3);
        Bisa::HitCollection narrowMuonCandidates = hitSelector_.restrictHitsOnLayer(muonCandidates.hits(), 1);
        Bisa::HitCollection muonTimeCorrelated = hitSelector_.featureCorrelated(firstHits, muonCandidates, SelectorFilter::in_time_hits);
        Bisa::HitCollection muonSpaceCorrelated = hitSelector_.featureCorrelated(firstHits, muonCandidates, SelectorFilter::adjacent_hits_across_layers);
        
        Bisa::HitCollection randomNoise = firstHits - muonTimeCorrelated;

        Bisa::HitCollection hasHitToRight = hitSelector_.hasHitToRight(muonCandidates.hits());
        Bisa::HitCollection hasHitToLeft = hitSelector_.hasHitToLeft(muonCandidates.hits());

        Bisa::HitCollection underflowWidth = hitSelector_.equalWidth(firstHits, 15);

        // Plot event display for 100 events
        if (muon_event_counter_ < 100)
        { 
            BA_TRACE("Event: {}\tNumber of hits: {}", hits_.trigger_id(), firstHits.size());
            EventSummary es4("first_100_events", config_);
            es4.configureAllHits(hits_);
            es4.addHits(hits_, kBlack);
            es4.addHits(muonCandidates.hits(), kRed);
            es4.addHits(randomNoise, kGreen);
            muon_event_counter_++;
        }

        // Plot event display for 100 events
        if (underflowWidth.size() > 0 && event_counter_ < 100)
        { 
            EventSummary es4("width_underflow", config_);
            es4.configureAllHits(hits_);
            es4.addHits(hits_, kBlack);
            es4.addHits(underflowWidth, kRed);
            event_counter_++;
        }



        // Hit distributions
        all_first_hd_.addHits(firstHits);
        muon_hd_.addHits(muonCandidates.hits());
        exlusive_muon_hd_.addHits(exlusiveMuonCandidates.hits());
        random_noise_hd_.addHits(randomNoise);
        noise_burst_hd_.addHits(noiseBursts.hits());
        number_of_hits_.fillHits(hits_);

        // Width distributions
        all_first_wd_.addHits(firstHits);
        // narrowMuonCandidates = muonCandidates.hits() & narrowMuonCandidates;
        // muon_wd_.addHits(narrowMuonCandidates);
        muon_wd_.addHits(narrowMuonCandidates);
        random_noise_wd_.addHits(randomNoise);
        noise_burst_wd_.addHits(noiseBursts.hits());
        Bisa::HitCollection crosstalk_hits = hitSelector_.restrictHitsOnTDC(firstHits, 2);
        // crosstalk_hits = crosstalk_hits - hitSelector_.equalWidth(crosstalk_hits, 15);
        // crosstalk_hits = hitSelector_.restrictHitsOnTDC(crosstalk_hits, 2);
        crosstalk_wd_.addHits(crosstalk_hits);

        // Cluster size
        for (auto &&f : spaceClusters)
        {
            cluster_size_.addHits(f);
        }

        // Calculate timing resolution
        if (noiseBursts.size() == 0)
        {
            timing_resolution_.calculateMeanTime(muonCandidates.hits());
            timing_resolution_.fillHits(muonSpaceCorrelated);
        }

        // Mapping plots
        Bisa::HitCollection twoHitsPerTDC = hitSelector_.restrictHitsOnTDC(firstHits, 2);
        twoHitsPerTDC = twoHitsPerTDC - hitSelector_.equalWidth(twoHitsPerTDC, 15); // Remove all crosstalk hits
        twoHitsPerTDC = hitSelector_.restrictHitsOnTDC(twoHitsPerTDC, 2);
        Bisa::HitCollection restrictedHitsToRight = twoHitsPerTDC & hasHitToRight;
        Bisa::HitCollection restrictedHitsToLeft = twoHitsPerTDC & hasHitToLeft;
        adjacent_hits_left_.addHits(restrictedHitsToLeft);
        adjacent_hits_right_.addHits(restrictedHitsToRight);

        cable_mapping_.addHits(firstHits);

        // Latency
        Bisa::HitCollection smallestBcidMuonCandidates = hitSelector_.smallestBcid(muonCandidates.hits());
        latency_.addHits(smallestBcidMuonCandidates);
        event_bcid_dist_.addHits(firstHits);

        // // Efficiency calculation
        // unsigned int fixed1 = 1;
        // unsigned int fixed2 = 2;
        // unsigned int changing = 0;
        // if (noiseBursts.hits().size() == 0)
        // {
        //     for (auto &&hit1 : *hits_)
        //     {
        //         if (config_.layer(hit1.tdc()) == fixed1)
        //         {
        //             for (auto &&hit2 : *hits_)
        //             {
        //                 if (config_.layer(hit2.tdc()) == fixed2)
        //                 {
        //                     if (std::abs((config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel())) - 
        //                                 (config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel()))) < 2 &&
        //                         config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()) && 
        //                         config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
        //                     {
        //                         efficiency_denom_0_++;

        //                         // Check for 3rd hit
        //                         for (auto &&hit3 : *hits_)
        //                         {
        //                             if (config_.chamber(hit3.tdc()) == config_.chamber(hit1.tdc()))
        //                             {
        //                                 if (config_.layer(hit3.tdc()) == changing &&
        //                                     config_.coordinate(hit3.tdc()) == config_.coordinate(hit1.tdc()))
        //                                 {
        //                                     // if (std::abs(config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel()) - 
        //                                     //             config_.orientation(hit3.tdc())*32+config_.strip(hit3.channel())) < 3 &&
        //                                     //     std::abs(config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel()) - 
        //                                     //             config_.orientation(hit3.tdc())*32+config_.strip(hit3.channel())) < 3)
        //                                     // {
        //                                         efficiency_num_0_++;
        //                                         break;
        //                                     // }
        //                                 }
        //                             }
        //                         }
                                
        //                     }
        //                 }
                        
        //             }
        //         }            
        //     }           
        // }
        // fixed1 = 0;
        // fixed2 = 2;
        // changing = 1;
        // if (noiseBursts.hits().size() == 0)
        // {
        //     for (auto &&hit1 : *hits_)
        //     {
        //         if (config_.layer(hit1.tdc()) == fixed1)
        //         {
        //             for (auto &&hit2 : *hits_)
        //             {
        //                 if (config_.layer(hit2.tdc()) == fixed2)
        //                 {
        //                     if (std::abs((config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel())) - 
        //                                 (config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel()))) < 2 &&
        //                         config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()) && 
        //                         config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
        //                     {
        //                         efficiency_denom_1_++;

        //                         // Check for 3rd hit
        //                         for (auto &&hit3 : *hits_)
        //                         {
        //                             if (config_.chamber(hit3.tdc()) == config_.chamber(hit1.tdc()))
        //                             {
        //                                 if (config_.layer(hit3.tdc()) == changing &&
        //                                     config_.coordinate(hit3.tdc()) == config_.coordinate(hit1.tdc()))
        //                                 {
        //                                     // if (std::abs(config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel()) - 
        //                                     //             config_.orientation(hit3.tdc())*32+config_.strip(hit3.channel())) < 3 &&
        //                                     //     std::abs(config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel()) - 
        //                                     //             config_.orientation(hit3.tdc())*32+config_.strip(hit3.channel())) < 3)
        //                                     // {
        //                                         efficiency_num_1_++;
        //                                         break;
        //                                     // }
        //                                 }
        //                             }
        //                         }
                                
        //                     }
        //                 }
                        
        //             }
        //         }            
        //     }           
        // }
        // fixed1 = 0;
        // fixed2 = 1;
        // changing = 2;
        // if (noiseBursts.hits().size() == 0)
        // {
        //     for (auto &&hit1 : *hits_)
        //     {
        //         if (config_.layer(hit1.tdc()) == fixed1)
        //         {
        //             for (auto &&hit2 : *hits_)
        //             {
        //                 if (config_.layer(hit2.tdc()) == fixed2)
        //                 {
        //                     if (std::abs((config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel())) - 
        //                                 (config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel()))) < 2 &&
        //                         config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()) && 
        //                         config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
        //                     {
        //                         efficiency_denom_2_++;

        //                         // Check for 3rd hit
        //                         for (auto &&hit3 : *hits_)
        //                         {
        //                             if (config_.chamber(hit3.tdc()) == config_.chamber(hit1.tdc()))
        //                             {
        //                                 if (config_.layer(hit3.tdc()) == changing &&
        //                                     config_.coordinate(hit3.tdc()) == config_.coordinate(hit1.tdc()))
        //                                 {
        //                                     // if (std::abs(config_.orientation(hit1.tdc())*32+config_.strip(hit1.channel()) - 
        //                                     //             config_.orientation(hit3.tdc())*32+config_.strip(hit3.channel())) < 3 &&
        //                                     //     std::abs(config_.orientation(hit2.tdc())*32+config_.strip(hit2.channel()) - 
        //                                     //             config_.orientation(hit3.tdc())*32+config_.strip(hit3.channel())) < 3)
        //                                     // {
        //                                         efficiency_num_2_++;
        //                                         break;
        //                                     // }
        //                                 }
        //                             }
        //                         }
                                
        //                     }
        //                 }
                        
        //             }
        //         }            
        //     }           
        // }

        // Update counters
        // event_counter_++;
        // if (muonCandidates.size() > 0)
        // {
        //     muon_event_counter_++;
        //     if (noiseBursts.hits().size() == 0)
        //     {
        //         clean_muon_event_counter_++;
        //     }

        //     unsigned int muon_eta_layer_sum_ = 0;
        //     unsigned int muon_phi_layer_sum_ = 0;
        //     for (auto &&hit : muonCandidates.hits())
        //     {
        //         if (config_.coordinate(hit.tdc()) == Bisa::ETA)
        //         {
        //             unsigned int layer = config_.layer(hit.tdc()) + 1;
        //             if (muon_eta_layers_.find(layer) == muon_eta_layers_.end())
        //             {
        //                 muon_eta_layer_sum_ += layer;
        //                 muon_eta_layers_.insert(layer);
        //             }
        //         }
        //         else
        //         {
        //             unsigned int layer = config_.layer(hit.tdc()) + 1;
        //             if (muon_phi_layers_.find(layer) == muon_phi_layers_.end())
        //             {
        //                 muon_phi_layer_sum_ += layer;
        //                 muon_phi_layers_.insert(layer);
        //             }
        //         }
        //     }
        //     muon_phi_layers_.clear();
        //     muon_eta_layers_.clear();
        //     muon_eta_layers_hit_[muon_eta_layer_sum_] += 1;
        //     muon_phi_layers_hit_[muon_phi_layer_sum_] += 1;

        // }
        // if (noiseBursts.size() > 0)
        // {
        //     noise_burst_event_counter_++;
        //     auto feature_it = noiseBursts.begin();
        //     int layer = config_.layer(feature_it->hits().begin()->tdc());
        //     for (; feature_it != noiseBursts.end(); feature_it++)
        //     {
        //         if (config_.layer(feature_it->hits().begin()->tdc()) != layer)
        //         {
        //             multiple_layer_noise_burst_event_counter_++;
        //             break;
        //         }
        //     }
        //     if (feature_it == noiseBursts.end())
        //     {
        //         single_layer_noise_burst_event_counter_++;
        //     }
        // }
    }

private:
    FeatureSelector featureSelector_;
    HitSelector hitSelector_;

    FirstHitsDistribution all_first_hd_;
    FirstHitsDistribution select_hd_;
    MuonHitsDistribution muon_hd_;
    MuonHitsDistribution exlusive_muon_hd_;
    RandomNoiseDistribution random_noise_hd_;
    NoiseBurstDistribution noise_burst_hd_;

    WidthDistribution all_first_wd_;
    WidthDistribution muon_wd_;
    WidthDistribution random_noise_wd_;
    WidthDistribution noise_burst_wd_;
    CrosstalkWidth crosstalk_wd_;

    TimingResolution timing_resolution_;

    NumberOfHits number_of_hits_;

    AdjacentHitsDistribution adjacent_hits_left_;
    AdjacentHitsDistribution adjacent_hits_right_;

    HitPairsTimeDistance cable_mapping_;

    FEClusterSize fe_cluster_size_;

    ClusterSize cluster_size_;

    LatencyTDCFPGA latency_;
    FPGABCIDDistribution event_bcid_dist_;

    // FirstHitOnStripSelector firstHitSelector;
    // SelectHitsSelector selectHitsSelector;
    // SpaceClusterSelector spaceClusterSelector;
    // LayerSpaceClusterSelector layerSpaceClusterSelector;
    // NoiseBurstSelector noiseBurstSelector;
    // TimeClusterSelector timeClusterSelector;
    // AdjacentHitsSelector adjacentHitsSelector;
    // MuonCandidateSelector muonCandidateSelector;
    // MuonCandidateNoTimeSelector muonCandidateNoTimeSelector;

    unsigned int noise_burst_event_counter_ = 0;
    unsigned int single_layer_noise_burst_event_counter_ = 0;
    unsigned int multiple_layer_noise_burst_event_counter_ = 0;
    
    int efficiency_num_0_ = 0;
    int efficiency_denom_0_ = 0;
    int efficiency_num_1_ = 0;
    int efficiency_denom_1_ = 0;
    int efficiency_num_2_ = 0;
    int efficiency_denom_2_ = 0;
    
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
