#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class FEClusterSize : Bisa::Plot
{
public:

    FEClusterSize(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , color_(Form("%s_color", name_), config)
     , profile_(Form("%s_profile", name_), config)
    {
        init();
    }

    ~FEClusterSize()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        for (auto &&hit : hits)
        {
            color_[hit.tdc()].Fill(config_.strip(hit.channel()), hits.size(), 1.0/hits.size());
            profile_[hit.tdc()].Fill(config_.strip(hit.channel()), hits.size(), 1.0/hits.size());
        }
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        color_.init(4, 0, 32, 32, 0, 32);
        profile_.init(4, 0, 32, 0, 32);
        
        gDirectory->cd("..");
    }

    void same_configure(TH2F& hist)
    {
        // for (int strip = 0; strip < 32; strip++)
        // {
        //     hist.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
        // }
        // hist.GetXaxis()->SetLabelSize(0.04);
    }

    void configure()
    {
        color_.configure();
        profile_.configure();

        // Configure color_ titles
        color_.for_each([&] (TH2F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Strip");
            hist.GetYaxis()->SetTitle("Cluster Size");
        });

        // Configure profile_ titles
        profile_.for_each([&] (TProfile& hist) {
            hist.GetXaxis()->SetTitle("Strip");
            hist.GetYaxis()->SetTitle("Cluster Size");
        });

        for (auto tdc = 0; tdc < 18; tdc++)
        {
            if (config_.coordinate(tdc) == Bisa::ETA)
            {
                color_[tdc].SetOption("COLZ");
                profile_[tdc].SetOption("COLZ");
            }
            else if (config_.orientation(tdc) == 0)
            {
                color_[tdc].SetOption("COL");
                profile_[tdc].SetOption("COL");
            }
            else
            {
                color_[tdc].SetOption("COLZ");
                profile_[tdc].SetOption("COLZ");
            }
        }
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/fec_cluster_size", true);

        auto props = Bisa::SummaryTdc<TH2F>::DrawProps();
        props.logz = true;

        color_.print("output/fec_cluster_size", props);
        profile_.print("output/fec_cluster_size");
    }

private:
    int strip(const Bisa::Hit& hit)
    {
        return config_.strip(hit.channel());
    }

private:
    Bisa::SummaryTdc<TH2F> color_;
    Bisa::SummaryTdc<TProfile> profile_;

};
