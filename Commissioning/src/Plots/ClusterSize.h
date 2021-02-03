#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class ClusterSize : Bisa::Plot
{
public:

    ClusterSize(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , summary_(Form("%s_color", name_), config)
    {
        init();
    }

    ~ClusterSize()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        summary_[hits.begin()->tdc()].Fill(hits.size());
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        summary_.init(32, 0, 32);
        
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

        // Configure color_ titles
        summary_.for_each([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Cluster Size");
            hist.GetYaxis()->SetTitle("Frequency");
        });
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/cluster_size", true);

        auto props = Bisa::SummaryTdc<TH1F>::DrawProps();
        props.logy = true;

        summary_.print("output/cluster_size", props);
    }

private:
    int strip(const Bisa::Hit& hit)
    {
        return config_.strip(hit.channel());
    }

private:
    Bisa::SummaryTdc<TH1F> summary_;

};
