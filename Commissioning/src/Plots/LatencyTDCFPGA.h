#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class LatencyTDCFPGA : Bisa::Plot
{
public:

    LatencyTDCFPGA(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , summary_(Form("%s_color", name_), config)
     , agg_(Form("%s_agg", name_), config)
    {
        init();
    }

    ~LatencyTDCFPGA()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        for (auto &&hit : hits)
        {
            auto value = hit.bcid_fpga() % 4096 - hit.bcid_tdc();
            summary_[hit.tdc()].Fill(value);
            agg_().Fill(value);
        }
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        summary_.init(100, 0, 100);
        agg_.init(100, 0, 100);
        
        gDirectory->cd("..");
    }

    void same_configure(TH1F& hist)
    {
        // for (int strip = 0; strip < 32; strip++)
        // {
        //     hist.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
        // }
        // hist.GetXaxis()->SetLabelSize(0.04);
    }

    void configure()
    {
        summary_.configure();
        agg_.configure();

        // Configure color_ titles
        summary_.for_each([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Latency (BCID Ticks)");
            hist.GetYaxis()->SetTitle("Frequency");
        });
        
        agg_.apply([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Latency (BCID Ticks)");
            hist.GetYaxis()->SetTitle("Frequency");
        });


    }

    void save()
    {
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(1111);

        gSystem->mkdir("output/latency", true);

        auto props = Bisa::SummaryTdc<TH1F>::DrawProps();
        props.logy = true;

        auto agg_props = Bisa::SummaryAgg<TH1F>::DrawProps();
        agg_props.logy = true;

        summary_.print("output/latency", props);
        agg_.print("output/latency", agg_props);
    }

private:
    int strip(const Bisa::Hit& hit)
    {
        return config_.strip(hit.channel());
    }

private:
    Bisa::SummaryTdc<TH1F> summary_;
    Bisa::SummaryAgg<TH1F> agg_;

};
