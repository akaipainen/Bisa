#include <Bisa.h>

#include "Features/SpaceCluster.h"
#include "Features/NoiseBurst.h"

#include "Plots/EventSummary.h"

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp()
    {
    }

    ~AnalysisApp()
    {
    }

    void Run() override
    {
        while (dataStream_->FillNextEvent())
        {
            // BA_CORE_INFO("Data found");
            for (auto &&h : *hits_)
            {
                // BA_CORE_INFO("{}", h.second->toString());
            }
            
            Step();
        }
    }

    void Step() override
    {
        SpaceClusterSelector spaceClusterSelector;
        Bisa::FeatureCollection spaceClusters = spaceClusterSelector(*hits_);

        NoiseBurstSelector noiseBurstSelector;
        Bisa::FeatureCollection noiseBursts = noiseBurstSelector(*hits_);

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
        }
        
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
    unsigned int event_counter_ = 0;
};

Bisa::Application* Bisa::CreateApplication()
{
    return new AnalysisApp();
}
