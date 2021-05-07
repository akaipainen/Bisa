#pragma once

#include <Bisa.h>

#include <TH1.h>
#include <TStyle.h>
#include <TSystem.h>

class CheckTriggerNumLayersHit : public Bisa::Plot
{
public:
    CheckTriggerNumLayersHit(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 2, 1, config)
     , eta_(Form("%s_eta", name_), config)
     , phi_(Form("%s_phi", name_), config)
     , or_(Form("%s_or", name_), config)
    {
        init();
    }

    ~CheckTriggerNumLayersHit()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        std::vector<int> eta_hits(3, 0);
        std::vector<int> phi_hits(3, 0);
        for (auto &&hit : hits)
        {
            if (config_.coordinate(hit.tdc()) == Bisa::ETA)
            {
                eta_hits[config_.layer(hit.tdc())]++;
            }
            if (config_.coordinate(hit.tdc()) == Bisa::PHI)
            {
                phi_hits[config_.layer(hit.tdc())]++;
            }
        }

        unsigned int eta = 0;
        unsigned int phi = 0;
        unsigned int or__ = 0;
        for (unsigned int i = 0; i < 3; i++)
        {
            eta += (eta_hits[i] > 0);
            phi += (phi_hits[i] > 0);
            or__ += (phi_hits[i] > 0 || eta_hits[i] > 0);
        }
        eta_().Fill(eta);
        phi_().Fill(phi);
        or_().Fill(std::max(eta, phi));
    }

    void configure()
    {
        eta_.configure();
        eta_.apply([&] (TH1F& hist) {
            hist.GetXaxis()->SetTitle("Number of Layers hit in event (ETA)");
            hist.GetYaxis()->SetTitle("Frequency");
        });

        phi_.configure();
        phi_.apply([&] (TH1F& hist) {
            hist.GetXaxis()->SetTitle("Number of Layers hit in event (PHI)");
            hist.GetYaxis()->SetTitle("Frequency");
        });

        or_.configure();
        or_.apply([&] (TH1F& hist) {
            hist.GetXaxis()->SetTitle("Number of Layers hit in event (ETA or PHI)");
            hist.GetYaxis()->SetTitle("Frequency");
        });
    }

    void save()
    {
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(0);

        gSystem->mkdir("output/check_trigger", true);

        auto agg_props = Bisa::SummaryAgg<TH1F>::DrawProps();
        agg_props.logy = true;

        eta_.print("output/check_trigger", agg_props);
        phi_.print("output/check_trigger", agg_props);
        or_.print("output/check_trigger", agg_props);
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

        eta_.init(4, 0, 4);
        phi_.init(4, 0, 4);
        or_.init(4, 0, 4);
        
        gDirectory->cd("..");
    }

private:
    // std::vector<TH1F*> tdcs_;
    Bisa::SummaryAgg<TH1F> eta_;
    Bisa::SummaryAgg<TH1F> phi_;
    Bisa::SummaryAgg<TH1F> or_;
    
};