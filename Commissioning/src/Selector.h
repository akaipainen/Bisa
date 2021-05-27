#pragma once

#include <Bisa.h>

// Helper functions to select features from features or hits
namespace Selector
{
    // Basic selector given hits and a 1-1 hit filter that returns true or false
    // depending on if these hits belong in same feature
    Bisa::FeatureCollection basicSelector(const Bisa::HitCollection& hits, const Bisa::Config& config, std::function<bool(const Bisa::Hit&, const Bisa::Hit&, const Bisa::Config&)> filter_pred);

    // Select features from feature collection with only a minimum feature size
    Bisa::FeatureCollection requireMinSize(const Bisa::FeatureCollection& features, unsigned int minSize, const Bisa::Config& config);
    Bisa::FeatureCollection requireMaxSize(const Bisa::FeatureCollection& features, unsigned int maxSize, const Bisa::Config& config);

    // Select features that span multiple layers
    Bisa::FeatureCollection requireMultipleLayers(const Bisa::FeatureCollection& features, unsigned int nLayers, const Bisa::Config& config);

    // Select features from feature collection with
    Bisa::FeatureCollection restrictMaxHitsPerLayer(const Bisa::FeatureCollection& features, unsigned int maxSize, const Bisa::Config& config);

};
