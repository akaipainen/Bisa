#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TString.h>
#include <TStyle.h>

class EventDisplay : public Bisa::Plot
{
public:
    EventDisplay(const char *name, const char *title, unsigned int max_number, Bisa::Experiment *experiment, const Bisa::Config &config)
     : Bisa::Plot(Form("%s_%d", name, event_counter_[name]), Form("%s %d", title, event_counter_[name]), experiment, config)
     , max_number_(max_number)
     , name__(name)
    {
        // Increment event counter
        if (event_counter_.find(name__) == event_counter_.end())
        {
            event_counter_[name__] = 0;
        } else {
            event_counter_[name__]++;
        }

        if (event_counter_[name__] < max_number_)
        {
            // Save to separate folder
            if (!gDirectory->cd(Form("/%s/%s", experiment_->name().Data(), name__)))
            {
                gDirectory->cd(Form("/%s", experiment_->name().Data()));
                gDirectory->mkdir(name__);
                gDirectory->cd(name__);
            }

            eta_ = {Form("%s_eta", name__), Form("%s_eta", title), 32, 0, 32, 3, 0, 3};
            phi_ = {Form("%s_phi", name__), Form("%s_phi", title), 64, 0, 64, 3, 0, 3};

            eta_.GetXaxis()->SetTitle("Strip #");
            eta_.GetYaxis()->SetTitle("Layer #");

            eta_.GetXaxis()->SetTitle("Strip #");
            eta_.GetYaxis()->SetTitle("Layer #");
        }
    }

    ~EventDisplay() 
    {
        if (event_counter_[name__] < max_number_)
        {
            gStyle->SetOptStat(0);

            canvas()->Divide(2, 1);

            canvas()->cd(1);
            eta_.SetMaximum(max_time_);
            eta_.Draw("COL");
            
            canvas()->cd(2);
            phi_.SetMaximum(max_time_);
            phi_.Draw("COLZ");
            
            print(Form("%s_%d", name__, trigger_id_), name__); // save plot to pdf
        }
    }

    void add_hits(const Bisa::HitCollection &hits)
    {    
        if (event_counter_[name__] < max_number_)
        {
            for (auto &&hit : hits)
            {
                double hit_time = config_.time(hit.bcid_tdc(), hit.fine_time());
                if (hit_time < min_time_)
                {
                    min_time_ = hit_time;
                }
                if (hit_time > max_time_)
                {
                    max_time_ = hit_time;
                }
            }
            min_time_ -= 0.1;

            for (auto &&hit : hits)
            {
                trigger_id_ = hit.trigger_id();
                if (config_.coordinate(hit.tdc()) == Bisa::ETA)
                {
                    eta_.Fill(config_.orientation(hit.tdc())*32 + config_.strip(hit.channel()), config_.layer(hit.tdc()), config_.time(hit.bcid_tdc(), hit.fine_time()) - min_time_);
                }
                else if (config_.coordinate(hit.tdc()) == Bisa::PHI)
                {
                    phi_.Fill(config_.orientation(hit.tdc())*32 + config_.strip(hit.channel()), config_.layer(hit.tdc()), config_.time(hit.bcid_tdc(), hit.fine_time()) - min_time_);
                }
            }
        }
    }

private:
    TH2F eta_;
    TH2F phi_;

    const char *name__;
    unsigned int trigger_id_ = 0;
    static std::map<TString, int> event_counter_;
    unsigned int max_number_;

    double min_time_ = 10000;
    double max_time_ = 0;
};