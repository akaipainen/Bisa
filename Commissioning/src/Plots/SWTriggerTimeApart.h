#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class SWTriggerTimeApart : Bisa::Plot
{
public:

    SWTriggerTimeApart(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , eta_(Form("%s_eta", name_), config)
     , phi_(Form("%s_phi", name_), config)
     , both_(Form("%s_both", name_), config)
    {
        init();
    }

    ~SWTriggerTimeApart()
    {
        configure();
        save();
    }

    void addFeatures(Bisa::FeatureCollection features, unsigned int time_apart)
    {
        bool eta = false;
        bool phi = false;
        for (auto &&feature : features)
        {
            if (!eta && config_.coordinate(feature.begin()->tdc()) == Bisa::ETA)
            {
                eta_().Fill(time_apart);
                eta = true;
            }
            if (!phi && config_.coordinate(feature.begin()->tdc()) == Bisa::PHI)
            {
                phi_().Fill(time_apart);
                phi = true;
            }
        }
        if (eta || phi)
        {
            both_().Fill(time_apart);
        }

        if (time_apart == 0) event_counter_++;
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        eta_.init(25, 0, 25);
        phi_.init(25, 0, 25);
        both_.init(25, 0, 25);
        
        gDirectory->cd("..");
    }

    void same_configure(TH1F& hist)
    {
        hist.Scale(1.0 / event_counter_);
        hist.GetXaxis()->SetTitle("Maximum time apart (ns)");
        hist.GetYaxis()->SetTitle("Trigger Efficieny");
    }

    void configure()
    {
        eta_.configure();
        eta_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        phi_.configure();
        phi_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        both_.configure();
        both_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });
    }

    void save()
    {
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(0);

        gSystem->mkdir("output/sw_trigger", true);

        auto props = Bisa::SummaryAgg<TH1F>::DrawProps();
        props.logy = true;

        eta_.print("output/sw_trigger", props);
        phi_.print("output/sw_trigger", props);
        both_.print("output/sw_trigger", props);
    }

private:
    Bisa::SummaryAgg<TH1F> eta_;
    Bisa::SummaryAgg<TH1F> phi_;
    Bisa::SummaryAgg<TH1F> both_;

    unsigned int event_counter_ = 0;

};
