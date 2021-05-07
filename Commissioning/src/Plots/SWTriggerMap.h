#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class SWTriggerMap : Bisa::Plot
{
public:

    SWTriggerMap(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , eta_(Form("%s_eta", name_), config)
     , phi_(Form("%s_phi", name_), config)
     , both_(Form("%s_both", name_), config)
    {
        init();
    }

    ~SWTriggerMap()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        bool eta = false;
        bool phi = false;
        for (auto &&hit1 : hits)
        {
            for (auto &&hit2 : hits)
            {
                if (config_.layer(hit1.tdc()) != config_.layer(hit2.tdc()))
                {
                    if (config_.coordinate(hit1.tdc()) == Bisa::ETA && config_.coordinate(hit2.tdc()) == Bisa::ETA)
                    {
                        eta = true;
                    }
                    if (config_.coordinate(hit1.tdc()) == Bisa::PHI && config_.coordinate(hit2.tdc()) == Bisa::PHI)
                    {
                        phi = true;
                    }
                }
            }
        }
        
        for (auto &&hit : hits)
        {
            if (eta)
            {
                eta_().Fill(config_.strip(hit.channel()));
            }
            if (phi)
            {
                phi_().Fill(config_.strip(hit.channel()));
            }
            if (eta && phi)
            {
                // both_().Fill(config_.strip)
            }
        }
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        eta_.init(32, 0, 32);
        phi_.init(64, 0, 64);
        both_.init(32, 0, 32, 64, 0, 64);
        
        gDirectory->cd("..");
    }

    void same_configure_1d(TH1F& hist)
    {
        hist.GetXaxis()->SetTitle("Strip");
        hist.GetYaxis()->SetTitle("Frequency");
    }

    void same_configure_2d(TH2F& hist)
    {
        hist.GetXaxis()->SetTitle("Phi");
        hist.GetYaxis()->SetTitle("Eta");
    }

    void configure()
    {
        eta_.configure();
        eta_.apply([&] (TH1F& hist) {
            same_configure_1d(hist);
        });

        phi_.configure();
        phi_.apply([&] (TH1F& hist) {
            same_configure_1d(hist);
        });

        both_.configure();
        both_.apply([&] (TH2F& hist) {
            same_configure_2d(hist);
        });
    }

    void save()
    {
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(0);

        gSystem->mkdir("output/sw_trigger", true);

        auto props_1d = Bisa::SummaryAgg<TH1F>::DrawProps();
        props_1d.logy = true;

        auto props_2d = Bisa::SummaryAgg<TH2F>::DrawProps();
        props_2d.logz = true;

        eta_.print("output/sw_trigger", props_1d);
        phi_.print("output/sw_trigger", props_1d);
        both_.print("output/sw_trigger", props_2d);
    }

private:
    Bisa::SummaryAgg<TH1F> eta_;
    Bisa::SummaryAgg<TH1F> phi_;
    Bisa::SummaryAgg<TH2F> both_;

    unsigned int event_counter_ = 0;

};
