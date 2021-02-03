#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TSystem.h>
#include <TStyle.h>

class FPGABCIDDistribution : public Bisa::Plot
{
public:

    FPGABCIDDistribution(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , agg_(Form("%s_agg", name_), config)
    {
        init();
    }

    virtual ~FPGABCIDDistribution()
    {
        configure();
        save();
    }

    virtual void addHits(Bisa::HitCollection hits)
    {
        unsigned int min_bcid = hits.begin()->bcid_fpga();
        unsigned int max_bcid = hits.begin()->bcid_fpga();
        for (auto &&hit : hits)
        {
            if (hit.bcid_fpga() < min_bcid)
            {
                min_bcid = hit.bcid_fpga();
            }
            if (hit.bcid_fpga() > max_bcid)
            {
                max_bcid = hit.bcid_fpga();
            }
        }

        agg_().Fill(max_bcid - min_bcid);
    }

    void init()
    {   
        gDirectory->cd(name_);

        agg_.init(10, 0, 10);

        gDirectory->cd("..");
    }

    void configure()
    {
        agg_.configure();

        agg_.apply([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("FPGA BCID Spread in Event");
            hist.GetYaxis()->SetTitle("Count");
        });
    }

    void save()
    {
        gStyle->SetOptStat(1111);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/latency", true);

        auto props = Bisa::SummaryAgg<TH1F>::DrawProps();
        props.options = "BAR E0"; // options
        props.logy = true;

        agg_.print("output/latency", props);
    }

protected:
    void same_configure(TH1F& hist)
    {
        // for (int strip = 0; strip < 32; strip++)
        // {
        //     hist.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
        // }
        // hist.GetXaxis()->SetLabelSize(0.1);

        hist.SetFillColor(16); // Bar color
        hist.SetLineColor(kBlack); // Error bar color
        hist.SetBarWidth(0.8); // Set bar width
        hist.SetBarOffset(0.1);
    }


public:
    Bisa::SummaryAgg<TH1F> agg_;
};
