#pragma once

#include <Bisa.h>

#include <TH1.h>
#include <TStyle.h>
#include <TSystem.h>

class NumberOfHits : public Bisa::Plot
{
public:
    NumberOfHits(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 2, 1, config)
    {
        init();
    }

    ~NumberOfHits()
    {
        configure();
        draw();
        for (auto &&tdc : tdcs_)
        {
            delete tdc;
        }
    }

    void fillHits(Bisa::HitCollection hits)
    {
        std::vector<int> tdc_hits(18, 0);
        for (auto &&hit : hits)
        {
            tdc_hits[hit.tdc()]++;
        }

        for (int i = 0; i < 18; i++)
        {
            tdcs_[i]->Fill(tdc_hits[i]);
        }
    }

    void configure()
    {
    }

    void draw()
    {
        canvas_->Divide(2, 1);

        gSystem->mkdir(Form("output/%s", name_), true);

        for (int tdc = 0; tdc < 18; tdc++)
        {

            // Set stat options for legend
            gStyle->SetOptFit(1);
            gStyle->SetOptStat(11);

            // Draw log scale plot
            canvas_->cd(1);
            tdcs_[tdc]->Draw();
            gPad->SetLogy(true);

            // Draw linear scale plot
            canvas_->cd(2);
            tdcs_[tdc]->Draw();
            gPad->SetLogy(false);

            // Save canvas to pdf
            canvas_->Print(Form("output/%s/tdcs_%d.pdf", name_, tdc)); 
            canvas_->Clear("D");
        }
    }


private:
    void init()
    {
        gDirectory->cd(name_);

        for (int tdc = 0; tdc < 18; tdc++)
        {
            tdcs_.push_back(new TH1F(Form("tdcs_%d", tdc), Form("TDC = %d", tdc),
                                        32, 0, 32)); // binning = number of ticks
            tdcs_.back()->GetXaxis()->SetTitle("Number of Hits");
            tdcs_.back()->GetYaxis()->SetTitle("Event Count");
        }
        
        gDirectory->cd("..");
    }

private:
    std::vector<TH1F*> tdcs_;

    double mean_time_eta = 0;
    double mean_time_phi = 0;
    
};