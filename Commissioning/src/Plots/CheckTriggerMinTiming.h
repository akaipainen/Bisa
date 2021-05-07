#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class CheckTriggerMinTiming : Bisa::Plot
{
public:
    CheckTriggerMinTiming(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , eta_(Form("%s_eta", name_), config)
     , phi_(Form("%s_phi", name_), config)
     , or_(Form("%s_or", name_), config)
     , and_(Form("%s_and", name_), config)
    {
        init();
    }

    ~CheckTriggerMinTiming()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        double eta = 1000;
        double phi = 1000;
        for (auto &&hit1 : hits)
        {
            for (auto &&hit2 : hits)
            {
                if (utils::hits_same_coord(hit1, hit2, config_)) // same coordinate
                {
                    if (!utils::hits_same_layer(hit1, hit2, config_)) // different layer
                    {
                        if (config_.coordinate(hit1.tdc()) == Bisa::ETA)
                        {
                            if (utils::hits_time_apart(hit1, hit2, config_) < eta)
                            {
                                eta = utils::hits_time_apart(hit1, hit2, config_);
                            }
                        }
                        else if (config_.coordinate(hit1.tdc()) == Bisa::PHI)
                        {
                            if (utils::hits_time_apart(hit1, hit2, config_) < phi)
                            {
                                phi = utils::hits_time_apart(hit1, hit2, config_);
                            }
                        }
                    }
                }
            }
        }
        eta_().Fill(eta);
        phi_().Fill(phi);
        or_().Fill(std::min(eta, phi));
        and_().Fill(std::max(eta, phi));

        event_counter_++;
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        eta_.init(50/config_.fine_time_resolution(), 0, 50);
        phi_.init(50/config_.fine_time_resolution(), 0, 50);
        or_.init(50/config_.fine_time_resolution(), 0, 50);
        and_.init(50/config_.fine_time_resolution(), 0, 50);
        
        gDirectory->cd("..");
    }

    void same_configure(TH1F& hist)
    {
        // hist.Scale(1.0 / event_counter_);
        hist.SetTitle("Minimum timing separation between two hits on different layers in an event");
        hist.GetXaxis()->SetTitle("Time [ns]");
        hist.GetYaxis()->SetTitle("Number of Events");
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

        and_.configure();
        and_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        or_.configure();
        or_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });
    }

    void save()
    {
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(0);

        gSystem->mkdir("output/check_trigger", true);

        auto props = Bisa::SummaryAgg<TH1F>::DrawProps();
        props.logy = true;

        eta_.print("output/check_trigger", props);
        phi_.print("output/check_trigger", props);
        or_.print("output/check_trigger", props);
        and_.print("output/check_trigger", props);
    }

private:
    Bisa::SummaryAgg<TH1F> eta_;
    Bisa::SummaryAgg<TH1F> phi_;
    Bisa::SummaryAgg<TH1F> or_;
    Bisa::SummaryAgg<TH1F> and_;

    unsigned int event_counter_ = 0;

};
