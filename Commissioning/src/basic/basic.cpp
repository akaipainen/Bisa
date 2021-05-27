#include "basic.h"

#include "event_display.h"

Basic::Basic(const char *name, TTree *tree, const Bisa::Config &config)
 : Bisa::Experiment(name, tree, config)
 , muon_strip_dist_("muon_strip_distribution", "Muon Strip Distribution", this, config)
 , strip_dist_("strip_distribution", "Hit Strip Distribution", this, config)
 , channel_dist_("channel_distribution", "Channel Strip Distribution", this, config)
 , timing_diff_("tdc_timing_difference", "TDC Timing Difference for all Pairs of Hits", this, config)
 , tdc_fpga_latency_("tdc_fpga_latency", "TDC-FPGA BCID Latency for all Pairs of Hits", this, config)
{
    tree->Branch("trigger_id", &trigger_id_);
    tree->Branch("num_hits", &num_hits_);
    tree->Branch("tdc", &tdc_);
    tree->Branch("strip", &strip_);
    tree->Branch("channel", &channel_);
    tree->Branch("width", &width_);
    tree->Branch("fine_time", &fine_time_);
    tree->Branch("bcid_tdc", &bcid_tdc_);
    tree->Branch("bcid_fpga", &bcid_fpga_);
    tree->Branch("felix_counter", &felix_counter_);
}

Basic::~Basic()
{
}

void Basic::add_hits(const Bisa::HitCollection &hits)
{
    // Fill TTree branch information for hits
    tdc_.clear();
    strip_.clear();
    channel_.clear();
    width_.clear();
    fine_time_.clear();
    bcid_tdc_.clear();
    bcid_fpga_.clear();
    felix_counter_.clear();
    num_hits_ = hits.size();
    trigger_id_ = hits.trigger_id();
    for (auto &&hit : hits)
    {
        tdc_.push_back(hit.tdc());
        strip_.push_back(hit.strip());
        channel_.push_back(hit.channel());
        width_.push_back(hit.width());
        fine_time_.push_back(hit.fine_time());
        bcid_tdc_.push_back(hit.bcid_tdc());
        bcid_fpga_.push_back(hit.bcid_fpga());
        felix_counter_.push_back(hit.felix_counter());
    }

    // Create plots
    EventDisplay event_display = {"event_display", "Event Display", 100, this, config_};
    event_display.add_hits(hits);

    Bisa::FeatureCollection muon_hits = Selector::basicSelector(hits, config_, SelectorFilter::muon_candidate);
    muon_hits = Selector::requireMultipleLayers(muon_hits, 2, config_);
    muon_hits = Selector::restrictMaxHitsPerLayer(muon_hits, 3, config_);
    muon_strip_dist_.add_hits(muon_hits.hits());

    strip_dist_.add_hits(hits);
    channel_dist_.add_hits(hits);
    timing_diff_.add_hits(hits);
    tdc_fpga_latency_.add_hits(hits);
}
