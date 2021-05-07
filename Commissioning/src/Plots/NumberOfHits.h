#pragma once

#include <Bisa.h>

#include <TH1.h>
#include <TStyle.h>
#include <TSystem.h>

class NumberOfHits : public Bisa::Plot
{
public:
    NumberOfHits(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 2, 1, config)
     , nTdcs_(Form("%s_tdcs", name_), config)
     , agg_(Form("%s_agg", name_), config)
     , layers_(Form("%s_layers", name_), config)
    {
        init();
    }

    ~NumberOfHits()
    {
        configure();
        save();
        // for (auto &&tdc : tdcs_)
        // {
        //     delete tdc;
        // }
    }

    void addHits(Bisa::HitCollection hits)
    {
        std::vector<int> tdc_hits(18, 0);
        std::vector<int> layer_hits(12, 0);
        for (auto &&hit : hits)
        {
            int tdc = hit.tdc();
            tdc_hits[tdc]++;
            layer_hits[6 * (config_.chamber(tdc) == 8) + 3 * config_.coordinate(tdc) + config_.layer(tdc)]++;
        }

        unsigned int hit_tdcs = 0;
        for (auto &&tdc : tdc_hits)
        {
            if (tdc > 0)
            {
                hit_tdcs++;
            }
        }
        nTdcs_().Fill(hit_tdcs);

        for (int i = 0; i < 18; i++)
        {
            // tdcs_[i]->Fill(tdc_hits[i]);
            if (i < 12)
            {
                layers_[i].Fill(layer_hits[i]);
            }
        }

        agg_().Fill(hits.size());
    }

    void configure()
    {
        nTdcs_.configure();
        nTdcs_.apply([&] (TH1F& hist) {
            hist.GetXaxis()->SetTitle("Number of TDCs Hit");
            hist.GetYaxis()->SetTitle("Number of Events");
        });

        agg_.configure();
        agg_.apply([&] (TH1F& hist) {
            hist.GetXaxis()->SetTitle("Number of hits in event");
            hist.GetYaxis()->SetTitle("Frequency");
        });

        layers_.for_each([&] (TH1F& hist) {
            hist.GetXaxis()->SetTitle("Number of hits in event (on layer)");
            hist.GetYaxis()->SetTitle("Frequency");
        });
    }

    void save()
    {
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(0);

        gSystem->mkdir("output/number_of_hits", true);

        auto agg_props = Bisa::SummaryAgg<TH1F>::DrawProps();
        agg_props.logy = true;

        agg_.print("output/number_of_hits", agg_props);
        nTdcs_.print("output/number_of_hits", agg_props);

        auto layer_props = Bisa::SummaryLayer<TH1F>::DrawProps();
        layer_props.logy = true;

        layers_.print("output/number_of_hits", layer_props);
    }


private:
    void init()
    {
        gDirectory->cd(name_);

        // for (int tdc = 0; tdc < 18; tdc++)
        // {
        //     tdcs_.push_back(new TH1F(Form("tdcs_%d", tdc), Form("TDC = %d", tdc),
        //                                 32, 0, 32)); // binning = number of ticks
        //     tdcs_.back()->GetXaxis()->SetTitle("Number of Hits");
        //     tdcs_.back()->GetYaxis()->SetTitle("Event Count");
        // }

        nTdcs_.init(18, 0, 18);
        layers_.init(100, 0, 100);
        agg_.init(100, 0, 100);
        
        gDirectory->cd("..");
    }

private:
    // std::vector<TH1F*> tdcs_;
    Bisa::SummaryAgg<TH1F> nTdcs_;z
    Bisa::SummaryAgg<TH1F> agg_;
    Bisa::SummaryLayer<TH1F> layers_;

    double mean_time_eta = 0;
    double mean_time_phi = 0;
    
};