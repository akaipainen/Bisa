#include <Bisa.h>

#include "Features/SpaceCluster.h"
#include "Features/NoiseBurst.h"
#include "Features/TimeCluster.h"
#include "Features/FirstHitOnStrip.h"

#include "Plots/EventSummary.h"
#include "Plots/HitDistribution.h"
#include "Plots/WidthDistribution.h"

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp()
    : all_first_hd_("all_first_hits", 1)
    , muon_hd_("muon_hits", 1/(10*60*60))
    , random_noise_hd_("random_noise_hits", 1/(0.0000016*85000))
    , noise_burst_hd_("noise_burst_hits", 1)
    , all_first_wd_("all_first_width")
    , muon_wd_("muon_width")
    , random_noise_wd_("random_noise_width")
    , noise_burst_wd_("noise_burst_width")
    {
    }

    ~AnalysisApp()
    {
    }

    void Run() override
    {
        while (dataStream_->FillNextEvent())
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

        Bisa::FeatureCollection spaceTimeCluster = Bisa::FeatureCollection::pairwise_intersect(timeClusters, spaceClusters);

        Bisa::HitCollection randomNoise = (firstHits - timeClusters.hits()) - spaceClusters.hits();

        if (event_counter_++ < 100)
        {
            EventSummary es1("events_noise_burst");
            es1.configureAllHits(*hits_);
            es1.addHits(*hits_, kBlack);
            es1.addHits(noiseBursts.hits(), kRed);
            
            EventSummary es2("events_space_cluster");
            es2.configureAllHits(*hits_);
            es2.addHits(*hits_, kBlack);
            es2.addHits(spaceClusters.hits(), kRed);
            
            EventSummary es3("events_space_time_cluster");
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
        
        // BA_TRACE(spaceClusters);

        // SparkSelector sparkSelector;
        // FirstHitSelector firstHitSelector;

        // FeatureCollection spark_selector = sparkSelector(allHits);

        // Bisa::HitCollection sparksHits = Bisa::HitCollection().add(sparks.Hit());

        // FeatureCollection space_clusters = SpaceClustersSelector();

        // Bisa::HitCollection spark_filter = Bisa::HitFilter(m_Hits, sparks, remove=true);

        // Bisa::HitCollection clusters = ClustersSelector(spark_filter);

        // Bisa::HitFilter();
    }

private:
    HitDistribution all_first_hd_;
    HitDistribution muon_hd_;
    HitDistribution random_noise_hd_;
    HitDistribution noise_burst_hd_;

    WidthDistribution all_first_wd_;
    WidthDistribution muon_wd_;
    WidthDistribution random_noise_wd_;
    WidthDistribution noise_burst_wd_;

    FirstHitOnStripSelector firstHitSelector;
    SpaceClusterSelector spaceClusterSelector;
    NoiseBurstSelector noiseBurstSelector;
    TimeClusterSelector timeClusterSelector;

    unsigned int event_counter_ = 0;
};

Bisa::Application* Bisa::CreateApplication()
{
    return new AnalysisApp();
}
