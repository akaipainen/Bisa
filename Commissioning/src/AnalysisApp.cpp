#include <Bisa.h>

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
        // FeatureCollection sparks = SparkSelector(HitCollection m_Hits);

        // HitCollection spark_filter = Bisa::HitCollection().add(space_clusters.Hit() - sparks.hits());

        // FeatureCollection space_clusters = SpaceClustersSelector();

        // HitCollection spark_filter = Bisa::HitFilter(m_Hits, sparks, remove=true);

        // HitCollection clusters = ClustersSelector(spark_filter);

        // Bisa::HitFilter()


    }
};

Bisa::Application* Bisa::CreateApplication()
{
    return new AnalysisApp();
}
