#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class SWTriggerStripDistance : Bisa::Plot
{
public:
    SWTriggerStripDistance(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , eta_(Form("%s_eta", name_), config)
     , phi_(Form("%s_phi", name_), config)
     , or_(Form("%s_etaorphi", name_), config)
     , and_(Form("%s_etaandphi", name_), config)
    {
        init();
    }

    ~SWTriggerStripDistance()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        unsigned int eta = 100;
        unsigned int phi = 100;
        for (auto &&hit1 : hits)
        {
            for (auto &&hit2 : hits)
            {
                if (utils::hits_same_coord(hit1, hit2, config_))
                {
                    if (utils::hits_time_apart(hit1, hit2, config_, 25))
                    {
                        if (!utils::hits_same_layer(hit1, hit2, config_))
                        {
                            unsigned int dist = std::abs(utils::hits_signed_dist_apart(hit1, hit2, config_));
                            if (config_.coordinate(hit1.tdc()) == Bisa::ETA)
                            {
                                if (dist < eta)
                                {
                                    eta = dist;
                                }
                            }
                            else
                            {
                                if (dist < phi)
                                {
                                    phi = dist;
                                }
                            }
                        }
                    }
                }
            }
        }
        eta_().Fill(eta);
        phi_().Fill(phi);
        or_().Fill(std::min(eta, phi));
        if (eta != 100 && phi != 100)
        {
            and_().Fill(std::max(eta, phi));
        }

        event_counter_++;
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        eta_.init(10, 0, 10);
        phi_.init(10, 0, 10);
        or_.init(10, 0, 10);
        and_.init(10, 0, 10);
        
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
        eta_.configure();
        eta_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        phi_.configure();
        phi_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        or_.configure();
        or_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });

        and_.configure();
        and_.apply([&] (TH1F& hist) {
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
        or_.print("output/sw_trigger", props);
        and_.print("output/sw_trigger", props);
    }

private:
    Bisa::SummaryAgg<TH1F> eta_;
    Bisa::SummaryAgg<TH1F> phi_;
    Bisa::SummaryAgg<TH1F> or_;
    Bisa::SummaryAgg<TH1F> and_;

    unsigned int event_counter_ = 0;

};
