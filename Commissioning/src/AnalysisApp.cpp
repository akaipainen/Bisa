#include <Bisa.h>

#include "Features/SpaceCluster.h"
#include "Features/NoiseBurst.h"
#include "Features/TimeCluster.h"
#include "Features/FirstHitOnStrip.h"
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

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp(const Bisa::Config& config)
    : Bisa::Application(config)
    , all_first_hd_("all_first_hits", config)
    , muon_hd_("muon_hits", config)
    , random_noise_hd_("random_noise_hits", config)
    , noise_burst_hd_("noise_burst_hits", config)
    , all_first_wd_("all_first_width", config)
    , muon_wd_("muon_width", config)
    , random_noise_wd_("random_noise_width", config)
    , noise_burst_wd_("noise_burst_width", config)
    , timing_resolution_("timing_resolution", config)
    , adjacent_hits_("adjacent_hits", config)
    , cable_mapping_("cable_mapping", config)
    , firstHitSelector(config)
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
        while (dataStream_->fill_next_event())
        {        
            Step();
        }
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

        Bisa::FeatureCollection adjacentHits = adjacentHitsSelector(firstHits - noiseBursts.hits());
        
        if (noise_burst_event_counter_ < 100)
        {
            EventSummary es1("events_noise_burst", config_);
            es1.configureAllHits(*hits_);
            es1.addHits(*hits_, kBlack);
            es1.addHits(noiseBursts.hits(), kRed);
            noise_burst_event_counter_++;
        }

        if (muon_event_counter_ < 100)
        { 
            EventSummary es4("events_muon_candidates", config_);
            es4.configureAllHits(*hits_);
            es4.addHits(*hits_, kBlack);
            es4.addHits(muonCandidates.hits(), kRed);
            muon_event_counter_++;
        }

        all_first_hd_.addHits(firstHits);
        muon_hd_.addHits(muonCandidates.hits());
        random_noise_hd_.addHits(randomNoise);
        noise_burst_hd_.addHits(noiseBursts.hits());

        all_first_wd_.addHits(firstHits);
        muon_wd_.addHits(muonCandidates.hits());
        random_noise_wd_.addHits(randomNoise);
        noise_burst_wd_.addHits(noiseBursts.hits());

        if (noiseBursts.size() == 0)
        {
            timing_resolution_.calculateMeanTime(muonCandidates.hits());
            timing_resolution_.fillHits(muonSpaceCandidates.hits());
        }

        adjacent_hits_.addHits(adjacentHits.hits());

        cable_mapping_.addHits(firstHits);
    }

private:
    FirstHitsDistribution all_first_hd_;
    MuonHitsDistribution muon_hd_;
    RandomNoiseDistribution random_noise_hd_;
    NoiseBurstDistribution noise_burst_hd_;

    WidthDistribution all_first_wd_;
    WidthDistribution muon_wd_;
    WidthDistribution random_noise_wd_;
    WidthDistribution noise_burst_wd_;

    TimingResolution timing_resolution_;

    AdjacentHitsDistribution adjacent_hits_;

    HitPairsTimeDistance cable_mapping_;

    FirstHitOnStripSelector firstHitSelector;
    SpaceClusterSelector spaceClusterSelector;
    NoiseBurstSelector noiseBurstSelector;
    TimeClusterSelector timeClusterSelector;
    AdjacentHitsSelector adjacentHitsSelector;
    MuonCandidateSelector muonCandidateSelector;
    MuonCandidateNoTimeSelector muonCandidateNoTimeSelector;

    unsigned int muon_event_counter_ = 0;
    unsigned int noise_burst_event_counter_ = 0;
};

Bisa::Application* Bisa::CreateApplication(const Bisa::Config& config)
{
    return new AnalysisApp(config);
}
