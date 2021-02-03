#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class CrosstalkWidth : Bisa::Plot
{
public:

    CrosstalkWidth(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , color_(Form("%s_color", name_), config)
     , agg_(Form("%s_agg", name_), config)
    {
        init();
    }

    ~CrosstalkWidth()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        for (auto hit_it = hits.begin(); hit_it != hits.end(); hit_it++)
        {
            for (auto next_hit_it = next(hit_it); next_hit_it != hits.end(); next_hit_it++)
            {
                if (hit_it->tdc() == next_hit_it->tdc())
                {
                    color_[hit_it->tdc()].Fill(hit_it->width(), next_hit_it->width());
                    agg_().Fill(hit_it->width(), next_hit_it->width());
                }
            }
        }
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        color_.init(128, 0, 128, 128, 0, 128);
        agg_.init(128, 0, 128, 128, 0, 128);
        
        gDirectory->cd("..");
    }

    void same_configure(TH2F& hist)
    {
        // for (int strip = 0; strip < 32; strip++)
        // {
        //     hist.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
        // }

        hist.GetXaxis()->SetLabelSize(0.04);
        hist.GetYaxis()->SetLabelSize(0.04);
    }

    void configure()
    {
        color_.configure();

        // Configure color_ titles
        color_.for_each([&] (TH2F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Hit 1 Width");
            hist.GetYaxis()->SetTitle("Hit 2 Width");
        });

        agg_.apply([&] (TH2F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Hit 1 Width");
            hist.GetYaxis()->SetTitle("Hit 2 Width");
        });

        for (auto tdc = 0; tdc < 18; tdc++)
        {
            if (config_.coordinate(tdc) == Bisa::ETA)
            {
                color_[tdc].SetOption("COLZ");
            }
            else if (config_.orientation(tdc) == 0)
            {
                color_[tdc].SetOption("COL");
            }
            else
            {
                color_[tdc].SetOption("COLZ");
            }
        }
        agg_().SetOption("COLZ");
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/width_distribution", true);

        auto props = Bisa::SummaryTdc<TH2F>::DrawProps();
        props.logz = true;

        auto agg_props = Bisa::SummaryAgg<TH2F>::DrawProps();
        agg_props.logz = true;

        color_.print("output/width_distribution", props);
        agg_.print("output/width_distribution", agg_props);
    }

private:
    Bisa::SummaryTdc<TH2F> color_;
    Bisa::SummaryAgg<TH2F> agg_;

};
