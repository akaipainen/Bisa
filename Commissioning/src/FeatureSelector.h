#pragma once

#include <Bisa.h>

// Class to select features from features or hits
class FeatureSelector
{
public:
    FeatureSelector(const Bisa::Config& config)
     : config_(config) { }

    ~FeatureSelector() { }

    // Basic selector given hits and a 1-1 hit filter that returns true or false
    // depending on if these hits belong in same feature
    Bisa::FeatureCollection basicSelector(const Bisa::HitCollection& hits, std::function<bool(const Bisa::Hit&, const Bisa::Hit&, const Bisa::Config&)> filter_pred);

    // Select features from feature collection with only a minimum feature size
    Bisa::FeatureCollection requireMinSize(const Bisa::FeatureCollection& features, unsigned int minSize);
    Bisa::FeatureCollection requireMaxSize(const Bisa::FeatureCollection& features, unsigned int maxSize);

    // Select features that span multiple layers
    Bisa::FeatureCollection requireMultipleLayers(const Bisa::FeatureCollection& features, unsigned int nLayers);

    // Select features from feature collection with
    Bisa::FeatureCollection restrictMaxHitsPerLayer(const Bisa::FeatureCollection& features, unsigned int maxSize);

private:
    const Bisa::Config& config_;
};
