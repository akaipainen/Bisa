#include <Bisa.h>

#include "Features/SpaceCluster.h"
#include "Features/NoiseBurst.h"
#include "Features/TimeCluster.h"
#include "Features/FirstHitOnStrip.h"
#include "Features/AdjacentHits.h"

#include "Plots/EventSummary.h"
#include "Plots/HitDistribution.h"
#include "Plots/RandomNoiseDistribution.h"
#include "Plots/FirstHitsDistribution.h"
#include "Plots/MuonHitsDistribution.h"
#include "Plots/NoiseBurstDistribution.h"
#include "Plots/WidthDistribution.h"
#include "Plots/TimingResolution.h"
#include "Plots/AdjacentHitsDistribution.h"

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
    , firstHitSelector(config)
    , spaceClusterSelector(config)
    , noiseBurstSelector(config)
    , timeClusterSelector(config)
    , adjacentHitsSelector(config)
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

        Bisa::FeatureCollection spaceClusters = spaceClusterSelector(firstHits);

        Bisa::FeatureCollection noiseBursts = noiseBurstSelector(*hits_);
        
        Bisa::FeatureCollection timeClusters = timeClusterSelector(firstHits);

        Bisa::FeatureCollection spaceTimeCluster = timeClusters & spaceClusters;

        Bisa::HitCollection randomNoise = (firstHits - timeClusters.hits()) - spaceClusters.hits();

        Bisa::FeatureCollection adjacentHits = adjacentHitsSelector(firstHits - noiseBursts.hits());

        if (event_counter_++ < 100)
        {
            EventSummary es1("events_noise_burst", config_);
            es1.configureAllHits(*hits_);
            es1.addHits(*hits_, kBlack);
            es1.addHits(noiseBursts.hits(), kRed);
            
            EventSummary es2("events_space_cluster", config_);
            es2.configureAllHits(*hits_);
            es2.addHits(*hits_, kBlack);
            es2.addHits(spaceClusters.hits(), kRed);
            
            EventSummary es3("events_space_time_cluster", config_);
            es3.configureAllHits(*hits_);
            es3.addHits(*hits_, kBlack);
            es3.addHits(spaceTimeCluster.hits(), kRed);
            es3.addHits(randomNoise, kBlue);
        }

        all_first_hd_.addHits(firstHits);
        muon_hd_.addHits(spaceTimeCluster.hits());
        random_noise_hd_.addHits(randomNoise);
        noise_burst_hd_.addHits(noiseBursts.hits());

        all_first_wd_.addHits(firstHits);
        muon_wd_.addHits(spaceTimeCluster.hits());
        random_noise_wd_.addHits(randomNoise);
        noise_burst_wd_.addHits(noiseBursts.hits());

        timing_resolution_.calculateMeanTime(spaceTimeCluster.hits());
        timing_resolution_.fillHits(spaceClusters.hits());

        adjacent_hits_.addHits(adjacentHits.hits());
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

    FirstHitOnStripSelector firstHitSelector;
    SpaceClusterSelector spaceClusterSelector;
    NoiseBurstSelector noiseBurstSelector;
    TimeClusterSelector timeClusterSelector;
    AdjacentHitsSelector adjacentHitsSelector;

    unsigned int event_counter_ = 0;
};

Bisa::Application* Bisa::CreateApplication(const Bisa::Config& config)
{
    return new AnalysisApp(config);
}
