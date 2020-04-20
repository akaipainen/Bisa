#include <Bisa.h>

#include "Features/SpaceCluster.h"

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp()
    {
    }

    ~AnalysisApp()
    {

    }

    void Step() override
    {
        BA_INFO("Initializing Space Cluster Selector");
        SpaceClusterSelector spaceClusterSelector;
        BA_INFO("Creating Space Clusters");
        Bisa::FeatureCollection spaceClusters = spaceClusterSelector(*hits_);

        BA_INFO("Printing space clusters ({})", spaceClusters.size());
        for (auto itCluster = spaceClusters.begin(); itCluster != spaceClusters.end(); itCluster++)
        {
            BA_TRACE("New Cluster: ");
            for (auto itHit = itCluster->second->hits().begin(); itHit != itCluster->second->hits().end(); itHit++)
            {
                BA_TRACE("{}", itHit->second->toString());
            }
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
};

Bisa::Application* Bisa::CreateApplication()
{
    return new AnalysisApp();
}
