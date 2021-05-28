#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TString.h>
#include <TStyle.h>

class EventDisplay : public Bisa::Plot
{
public:
    EventDisplay(const char *name, const char *title, unsigned int max_number, Bisa::Experiment *experiment)
     : Bisa::Plot(Form("%s_%d", name, event_counter_[name]), Form("%s %d", title, event_counter_[name]), experiment)
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

            eta_7_ = {Form("%s_eta7", name__), Form("%s_eta7", title), 32, 0, 32, 3, 0, 3};
            phi_7_ = {Form("%s_phi7", name__), Form("%s_phi7", title), 64, 0, 64, 3, 0, 3};
            eta_8_ = {Form("%s_eta8", name__), Form("%s_eta8", title), 32, 0, 32, 3, 0, 3};
            phi_8_ = {Form("%s_phi8", name__), Form("%s_phi8", title), 64, 0, 64, 3, 0, 3};

            eta_7_.GetXaxis()->SetTitle("Strip #");
            eta_7_.GetYaxis()->SetTitle("Layer #");

            phi_7_.GetXaxis()->SetTitle("Strip #");
            phi_7_.GetYaxis()->SetTitle("Layer #");

            eta_8_.GetXaxis()->SetTitle("Strip #");
            eta_8_.GetYaxis()->SetTitle("Layer #");

            phi_8_.GetXaxis()->SetTitle("Strip #");
            phi_8_.GetYaxis()->SetTitle("Layer #");
        }
    }

    ~EventDisplay() 
    {
        if (event_counter_[name__] < max_number_)
        {
            gStyle->SetOptStat(0);

            canvas()->Divide(2, 2);

            canvas()->cd(1);
            eta_7_.SetMaximum(max_time_);
            eta_7_.Draw("COLZ");
            
            canvas()->cd(2);
            phi_7_.SetMaximum(max_time_);
            phi_7_.Draw("COLZ");

            canvas()->cd(3);
            eta_8_.SetMaximum(max_time_);
            eta_8_.Draw("COLZ");
            
            canvas()->cd(4);
            phi_8_.SetMaximum(max_time_);
            phi_8_.Draw("COLZ");
            
            print(Form("%s_%d", name__, trigger_id_), name__); // save plot to pdf
        }
    }

    void add_hits(const Bisa::HitCollection &hits)
    {    
        if (event_counter_[name__] < max_number_)
        {
            for (auto &&hit : hits)
            {
                double hit_time = hit.time();
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
                double time_weight = hit.time() - min_time_;

                if (hit.coordinate() == Bisa::ETA && hit.chamber() == 7)
                {
                    eta_7_.Fill(hit.strip(), hit.layer(), time_weight);
                }
                else if (hit.coordinate() == Bisa::ETA && hit.chamber() == 8)
                {
                    eta_8_.Fill(hit.strip(), hit.layer(), time_weight);
                }
                else if (hit.coordinate() == Bisa::PHI && hit.chamber() == 7)
                {
                    phi_7_.Fill(hit.strip(), hit.layer(), time_weight);
                }
                else if (hit.coordinate() == Bisa::PHI && hit.chamber() == 8)
                {
                    phi_8_.Fill(hit.strip(), hit.layer(), time_weight);
                }
            }
        }
    }

private:
    TH2F eta_7_;
    TH2F phi_7_;
    TH2F eta_8_;
    TH2F phi_8_;

    const char *name__;
    unsigned int trigger_id_ = 0;
    static std::map<TString, int> event_counter_;
    unsigned int max_number_;

    double min_time_ = 10000;
    double max_time_ = 0;
};