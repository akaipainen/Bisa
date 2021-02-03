#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TSystem.h>
#include <TStyle.h>

class WidthDistribution : Bisa::Plot
{
public:

    WidthDistribution(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , strip_(Form("%s_strip", name_), config)
     , channel_(Form("%s_channel", name_), config)
     , one_dimensional_(Form("%s_1d", name_), config)
    {
        init();
    }

    ~WidthDistribution()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        for (auto &&hit : hits)
        {
            strip_[hit.tdc()].Fill(strip(hit), hit.width());
            channel_[hit.tdc()].Fill(hit.channel(), hit.width());
            // if (hit.width() < 0 || hit.width() > 128) {
            //     std::cout << hit.width() << std::endl;
            // }
            one_dimensional_[hit.tdc()].Fill(hit.width());
        }
    }

    void init()
    {
        gDirectory->cd(name_);
        
        strip_.init(32, 0, 32, 128, 0, 128);
        channel_.init(32, 0, 32, 128, 0, 128);
        one_dimensional_.init(128, 0, 128);
        
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
        strip_.configure();
        channel_.configure();

        strip_.for_each([&] (TH2F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Strip");
            hist.GetYaxis()->SetTitle("Width");
        });
        channel_.for_each([&] (TH2F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Channel");
            hist.GetYaxis()->SetTitle("Width");
        });
        one_dimensional_.for_each([&] (TH1F& hist) {
            hist.GetXaxis()->SetTitle("Width");
            hist.GetYaxis()->SetTitle("Count");
        });

        for (auto tdc = 0; tdc < 18; tdc++)
        {
            if (tdc < 3)           
            {
                strip_[tdc].SetOption("COLZ");
                channel_[tdc].SetOption("COLZ");
                one_dimensional_[tdc].SetOption("COLZ");
            }
            else if (tdc % 2 == 1) 
            {
                strip_[tdc].SetOption("COL");
                channel_[tdc].SetOption("COL");
                one_dimensional_[tdc].SetOption("COL");
            }
            else if (tdc % 2 == 0)    
            {
                strip_[tdc].SetOption("COLZ");
                channel_[tdc].SetOption("COLZ");
                one_dimensional_[tdc].SetOption("COLZ");
            }
        }
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/width_distribution", true);

        auto props2 = Bisa::SummaryTdc<TH2F>::DrawProps();
        props2.logz = true;

        auto props1 = Bisa::SummaryTdc<TH1F>::DrawProps();
        props1.logy = true;

        strip_.print("output/width_distribution", props2);
        channel_.print("output/width_distribution", props2);
        one_dimensional_.print("output/width_distribution", props1);
    }

private:
    // TODO: Move to config file
    unsigned int strip(const Bisa::Hit& hit)
    {
        return config_.strip(hit.channel());
    }

private:
    Bisa::SummaryTdc<TH2F> strip_;
    Bisa::SummaryTdc<TH2F> channel_;
    Bisa::SummaryTdc<TH1F> one_dimensional_;

};
