#pragma once

#include <Bisa.h>

#include <TH1.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TStyle.h>
#include <TSystem.h>

class TimingResolution : public Bisa::Plot
{
public:
    TimingResolution(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 2, 1, config)
    {
        init();
    }

    ~TimingResolution()
    {
        configure();
        draw();
        for (auto &&tdc : tdcs_)
        {
            delete tdc;
        }
    }

    void calculateMeanTime(Bisa::HitCollection hits)
    {
        mean_time_eta = 0;
        mean_time_phi = 0;
        int count_eta = 0;
        int count_phi = 0;
        for (auto &&hit : hits)
        {
            if (coordinate(hit) == Bisa::ETA)
            {
                mean_time_eta += time(hit);
                count_eta++;
            }
            else
            {
                mean_time_phi += time(hit);
                count_phi++;
            }
        }
        mean_time_eta /= count_eta;
        mean_time_phi /= count_phi;
    }

    void fillHits(Bisa::HitCollection hits)
    {
        for (auto &&hit : hits)
        {
            if (coordinate(hit) == Bisa::ETA) tdcs_[hit.tdc()]->Fill(time(hit) - mean_time_eta);
            else tdcs_[hit.tdc()]->Fill(time(hit) - mean_time_phi);
        }
    }

    void configure()
    {
    }

    void draw()
    {
        canvas_->Divide(2, 1);

        gSystem->mkdir(Form("output/%s", name_), true);

        TF1* formula = new TF1("full_width_3/4_max", "gaus");
        for (int tdc = 0; tdc < 9; tdc++)
        {
            // Create full width half max gaussian fit
            full_width_r_max(*(tdcs_[tdc]), *formula, 0.2); // get full width 3/4 max
            TFitResultPtr r = tdcs_[tdc]->Fit(formula, "SRQ0");
            double mean = r->Parameter(1);
            tdcs_[tdc]->SetAxisRange(mean - 20, mean + 20);
            
            // Resize plot to zoom in to +/- 20ns around mean
            TF1 extrapolated(*formula);
            extrapolated.SetRange(mean - 20, mean + 20);
            extrapolated.SetLineStyle(3);

            // Set stat options for legend
            gStyle->SetOptFit(1);
            gStyle->SetOptStat(11);

            // Draw log scale plot
            canvas_->cd(1);
            tdcs_[tdc]->Draw();
            formula->Draw("SAME");
            extrapolated.Draw("SAME");
            gPad->SetLogy(true);

            // Draw linear scale plot
            canvas_->cd(2);
            tdcs_[tdc]->Draw();
            formula->Draw("SAME");
            extrapolated.Draw("SAME");
            gPad->SetLogy(false);

            // Save canvas to pdf
            canvas_->Print(Form("output/%s/tdcs_%d.pdf", name_, tdc)); 
            canvas_->Clear("D");
        }
    }


private:
    void init()
    {
        for (int tdc = 0; tdc < 9; tdc++)
        {
            tdcs_.push_back(new TH1F(Form("tdcs_%d", tdc), Form("TDC = %d", tdc),
                                        200/25*128, -100, 100)); // binning = number of ticks
            tdcs_.back()->GetXaxis()->SetTitle("Time [ns]");
            tdcs_.back()->GetYaxis()->SetTitle("Count");
        }
    }

    bool coordinate(const Bisa::Hit& hit)
    {
        return config_.coordinate(hit.tdc());
    }

    double time(const Bisa::Hit& hit)
    {
        return hit.bcid_tdc() * config_.bcid_resolution() + hit.fine_time() * config_.fine_time_resolution();
    }

    void full_width_r_max(const TH1 &hist, TF1 &formula, const double r=0.5)
    {
        double r_max_val = r * hist.GetMaximum();
        int lower_bin = hist.FindFirstBinAbove(r_max_val);
        int upper_bin = hist.FindLastBinAbove(r_max_val);
        double lower = hist.GetXaxis()->GetBinCenter(lower_bin);
        double upper = hist.GetXaxis()->GetBinCenter(upper_bin);
        formula.SetRange(lower, upper);
    }

private:
    std::vector<TH1F*> tdcs_;

    double mean_time_eta = 0;
    double mean_time_phi = 0;
    
};