#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class SWTriggerPotentialTriggers : Bisa::Plot
{
public:
    SWTriggerPotentialTriggers(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , eta_less_(Form("%s_eta_less", name_), config)
     , phi_less_(Form("%s_phi_less", name_), config)
     , or_less_(Form("%s_etaorphi_less", name_), config)
     , and_less_(Form("%s_etaandphi_less", name_), config)
     , eta_greater_(Form("%s_eta_greater", name_), config)
     , phi_greater_(Form("%s_phi_greater", name_), config)
     , or_greater_(Form("%s_etaorphi_greater", name_), config)
     , and_greater_(Form("%s_etaandphi_greater", name_), config)
    {
        init();
    }

    ~SWTriggerPotentialTriggers()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        unsigned int eta_less = 100;
        unsigned int phi_less = 100;
        unsigned int eta_greater = 100;
        unsigned int phi_greater = 100;
        for (auto &&hit1 : hits)
        {
            for (auto &&hit2 : hits)
            {
                if (utils::hits_same_coord(hit1, hit2, config_))
                {
                    if (!utils::hits_same_layer(hit1, hit2, config_))
                    {
                        unsigned int dist = std::abs(utils::hits_signed_dist_apart(hit1, hit2, config_));
                        if (config_.coordinate(hit1.tdc()) == Bisa::ETA)
                        {
                            if (utils::hits_time_apart(hit1, hit2, config_, 10))
                            {
                                if (dist < eta_less)
                                {
                                    eta_less = dist;
                                }
                            }
                            else
                            {
                                if (dist < eta_greater)
                                {
                                    eta_greater = dist;
                                }
                            }
                        }
                        else
                        {
                            if (utils::hits_time_apart(hit1, hit2, config_, 10))
                            {
                                if (dist < phi_less)
                                {
                                    phi_less = dist;
                                }
                            }
                            else
                            {
                                if (dist < phi_greater)
                                {
                                    phi_greater = dist;
                                }
                            }
                        }
                    }
                }
            }
        }
        eta_less_().Fill(eta_less);
        phi_less_().Fill(phi_less);
        eta_greater_().Fill(eta_greater);
        phi_greater_().Fill(phi_greater);

        or_less_().Fill(std::min(eta_less, phi_less));
        or_greater_().Fill(std::min(eta_greater, phi_greater));

        if (eta_less != 100 && phi_less != 100)
        {
            and_less_().Fill(std::max(eta_less, phi_less));
        }

        if (eta_greater != 100 && phi_greater != 100)
        {
            and_greater_().Fill(std::max(eta_less, phi_less));
        }

        event_counter_++;
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        eta_less_.init(10, 0, 10);
        phi_less_.init(10, 0, 10);
        or_less_.init(10, 0, 10);
        and_less_.init(10, 0, 10);

        eta_greater_.init(10, 0, 10);
        phi_greater_.init(10, 0, 10);
        or_greater_.init(10, 0, 10);
        and_greater_.init(10, 0, 10);
        
        gDirectory->cd("..");
    }

    void same_configure(TH1F& hist)
    {
        hist.Scale(1.0 / event_counter_);
        hist.GetXaxis()->SetTitle("Minimum strip separation");
        hist.GetYaxis()->SetTitle("Efficiency");
    }

    void configure()
    {
        eta_less_.configure();
        eta_less_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        phi_less_.configure();
        phi_less_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        or_less_.configure();
        or_less_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        and_less_.configure();
        and_less_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });
        
        eta_greater_.configure();
        eta_greater_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        phi_greater_.configure();
        phi_greater_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        or_greater_.configure();
        or_greater_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        and_greater_.configure();
        and_greater_.apply([&] (TH1F& hist) {
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

        eta_less_.print("output/sw_trigger", props);
        phi_less_.print("output/sw_trigger", props);
        or_less_.print("output/sw_trigger", props);
        and_less_.print("output/sw_trigger", props);
        
        eta_greater_.print("output/sw_trigger", props);
        phi_greater_.print("output/sw_trigger", props);
        or_greater_.print("output/sw_trigger", props);
        and_greater_.print("output/sw_trigger", props);
    }

private:
    Bisa::SummaryAgg<TH1F> eta_less_;
    Bisa::SummaryAgg<TH1F> phi_less_;
    Bisa::SummaryAgg<TH1F> or_less_;
    Bisa::SummaryAgg<TH1F> and_less_;

    Bisa::SummaryAgg<TH1F> eta_greater_;
    Bisa::SummaryAgg<TH1F> phi_greater_;
    Bisa::SummaryAgg<TH1F> or_greater_;
    Bisa::SummaryAgg<TH1F> and_greater_;

    unsigned int event_counter_ = 0;

};
