#include <Bisa.h>

#include "Features/SpaceCluster.h"
#include <TH1F.h>
#include <TCanvas.h>

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp()
     : strip_("Total hit rate")
     , canvas_(new TCanvas("test", "Test", 500, 300))
    {
        strip_.init(32, 0, 32);
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
        strip_.draw(canvas_);
        canvas_->Print("test.pdf");
    }

    void Step() override
    {
        // BA_INFO("Initializing Space Cluster Selector");
        SpaceClusterSelector spaceClusterSelector;
        // spaceClusterSelector.setCreateFeatureCallback([&](Bisa::Feature& f) {
            
        // });
        // BA_INFO("Creating Space Clusters");
        Bisa::FeatureCollection spaceClusters = spaceClusterSelector(*hits_);


        // BA_INFO("Printing space clusters ({})", spaceClusters.size());
        for (auto itCluster = spaceClusters.begin(); itCluster != spaceClusters.end(); itCluster++)
        {
            // BA_TRACE("New Cluster: ");
            for (auto itHit = itCluster->second->hits().begin(); itHit != itCluster->second->hits().end(); itHit++)
            {
                // BA_TRACE("{}", itHit->second->toString());
            }
        }
        
        const std::vector<int> strip_mapping = {
            0, 4, 8, 12, 16, 20, 24, 28,
            1, 5, 9, 13, 17, 21, 25, 29,
            2, 6,10, 14, 18, 22, 26, 30,
            3, 7,11, 15, 19, 23, 27, 31
        };

        // Add hits to graph
        for (auto &&hit : *hits_)
        {
            auto strip = strip_mapping[hit.second->channel];
            strip_[hit.second->tdc].Fill(strip);
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
    Bisa::SummaryTdc<TH1F> strip_;
    TCanvas* canvas_;
};

Bisa::Application* Bisa::CreateApplication()
{
    return new AnalysisApp();
}
