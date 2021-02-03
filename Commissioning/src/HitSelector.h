#pragma once

#include <Bisa.h>

class HitSelector
{
public:
    using FilterPredicate = std::function<bool(const Bisa::Hit&, const Bisa::Hit&, const Bisa::Config&)>;

    HitSelector(const Bisa::Config& config)
     : config_(config) { }
    
    ~HitSelector() { }

    // Require hits to be time correlated with a hit from a feature
    Bisa::HitCollection featureCorrelated(const Bisa::HitCollection& hits, 
                                          const Bisa::FeatureCollection& features, 
                                          FilterPredicate filter_pred);

    // Select hits left and right adjacent from muon candidates
    Bisa::HitCollection hasHitToRight(const Bisa::HitCollection& hits);
    Bisa::HitCollection hasHitToLeft(const Bisa::HitCollection& hits);

    // Restrict collection to only having N hits on a single layer or TDC (clean data)
    Bisa::HitCollection restrictHitsOnLayer(const Bisa::HitCollection& hits, unsigned int nHits);
    Bisa::HitCollection restrictHitsOnTDC(const Bisa::HitCollection& hits, unsigned int nHits);

    // Restrict collection to only having hits with width equal to width
    Bisa::HitCollection equalWidth(const Bisa::HitCollection& hits, unsigned int width);

    // Restrict collection to only having hits with the smallest bcid in the event (removing later hits)
    Bisa::HitCollection smallestBcid(const Bisa::HitCollection& hits);

private:
    const Bisa::Config& config_;
};