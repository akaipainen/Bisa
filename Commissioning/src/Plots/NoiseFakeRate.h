#pragma once

#include <Bisa.h>

#include <TGraph.h>
#include <TSystem.h>
#include <TStyle.h>

class NoiseFakeRate : public Bisa::Plot
{
public:

    NoiseFakeRate(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , bis7_eta_(Form("%s_bis7_eta", name_), config)
     , bis7_phi_(Form("%s_bis7_phi", name_), config)
     , bis8_eta_(Form("%s_bis8_eta", name_), config)
     , bis8_phi_(Form("%s_bis8_phi", name_), config)
     , bis7_eta_or_phi_(Form("%s_bis7_eta_or_phi", name_), config)
     , bis7_eta_and_phi_(Form("%s_bis7_eta_and_phi", name_), config)
     , bis8_eta_or_phi_(Form("%s_bis8_eta_or_phi", name_), config)
     , bis8_eta_and_phi_(Form("%s_bis8_eta_and_phi", name_), config)
     , bis78_(Form("%s_eta_or_phi", name_), config)
    {
        init();
    }

    virtual ~NoiseFakeRate()
    {
        configure();
        save();
    }

    virtual void addHits(Bisa::HitCollection hits)
    {
        // unsigned int min_bcid = hits.begin()->bcid_fpga();
        // unsigned int max_bcid = hits.begin()->bcid_fpga();
        // for (auto &&hit : hits)
        // {
        //     if (hit.bcid_fpga() < min_bcid)
        //     {
        //         min_bcid = hit.bcid_fpga();
        //     }
        //     if (hit.bcid_fpga() > max_bcid)
        //     {
        //         max_bcid = hit.bcid_fpga();
        //     }
        // }

        // agg_().Fill(max_bcid - min_bcid);
    }

    void init()
    {   
        gDirectory->cd(name_);

        double start = 50;
        double end = 300;
        unsigned int n = 100;

        double x[n], 
               eta7[n], phi7[n], 
               eta8[n], phi8[n], 
               etaorphi7[n], etaandphi7[n], 
               etaorphi8[n], etaandphi8[n], 
               all[n];
        for (unsigned int i = 0; i < n; i++)
        {
            x[i] = (end - start) * i / n + start;
            eta7[i] = (config_.strip_area_bis7_eta() * x[i]) * (config_.strip_area_bis7_eta() * x[i]) * 10e-9 * 32 * 3 * 3;
            phi7[i] = (config_.strip_area_bis7_phi() * x[i]) * (config_.strip_area_bis7_phi() * x[i]) * 10e-9 * 64 * 3 * 3;
            eta8[i] = (config_.strip_area_bis8_eta() * x[i]) * (config_.strip_area_bis8_eta() * x[i]) * 10e-9 * 24 * 3 * 3;
            phi8[i] = (config_.strip_area_bis8_phi() * x[i]) * (config_.strip_area_bis8_phi() * x[i]) * 10e-9 * 64 * 3 * 3;

            etaorphi7[i] = eta7[i] + phi7[i];
            etaandphi7[i] = eta7[i] * phi7[i] * 10e-9;

            etaorphi8[i] = eta8[i] + phi8[i];
            etaandphi8[i] = eta8[i] * phi8[i] * 10e-9;

            all[i] = etaandphi7[i] + etaandphi8[i];
        }

        bis7_eta_.init_(n, x, eta7);
        bis7_phi_.init_(n, x, phi7);
        bis8_eta_.init_(n, x, eta8);
        bis8_phi_.init_(n, x, phi8);
        bis7_eta_or_phi_.init_(n, x, etaorphi7);
        bis7_eta_and_phi_.init_(n, x, etaandphi7);
        bis8_eta_or_phi_.init_(n, x, etaorphi8);
        bis8_eta_and_phi_.init_(n, x, etaandphi8);
        bis78_.init_(n, x, all);

        gDirectory->cd("..");
    }

    void configure()
    {
        // bis7_eta_.configure();
        // bis7_phi_.configure();
        // bis8_eta_.configure();
        // bis8_phi_.configure();
        // bis7_eta_or_phi_.configure();
        // bis7_eta_and_phi_.configure();
        // bis8_eta_or_phi_.configure();
        // bis8_eta_and_phi_.configure();
        // eta_or_phi_.configure();

        bis7_eta_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
        bis7_phi_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
        bis8_eta_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
        bis8_phi_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
        bis7_eta_or_phi_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
        bis7_eta_and_phi_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
        bis8_eta_or_phi_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
        bis8_eta_and_phi_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
        bis78_.apply([&] (TGraph& graph) {
            same_configure(graph);

            graph.GetXaxis()->SetTitle("Noise rate [Hz/cm^2]");
            graph.GetYaxis()->SetTitle("Fake rate [Hz]");
        });
    }

    void save()
    {
        gStyle->SetOptStat(1111);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/noise_fake_rate", true);

        auto props = Bisa::SummaryAgg<TGraph>::DrawProps();
        props.options = "AC"; // options
        props.logy = false;

        bis7_eta_.print("output/noise_fake_rate", props);
        bis7_phi_.print("output/noise_fake_rate", props);
        bis8_eta_.print("output/noise_fake_rate", props);
        bis8_phi_.print("output/noise_fake_rate", props);
        bis7_eta_or_phi_.print("output/noise_fake_rate", props);
        bis7_eta_and_phi_.print("output/noise_fake_rate", props);
        bis8_eta_or_phi_.print("output/noise_fake_rate", props);
        bis8_eta_and_phi_.print("output/noise_fake_rate", props);
        bis78_.print("output/noise_fake_rate", props);
    }

protected:
    void same_configure(TGraph& graph)
    {
        // for (int strip = 0; strip < 32; strip++)
        // {
        //     graph.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
        // }
        // graph.GetXaxis()->SetLabelSize(0.1);

        // graph.SetFillColor(16); // Bar color
        // graph.SetLineColor(kBlack); // Error bar color
        // graph.SetBarWidth(0.8); // Set bar width
        // graph.SetBarOffset(0.1);
        // graph.SetMaximum(1.2 * graph.GetMaximum());
    }

public:
    Bisa::SummaryAgg<TGraph> bis7_eta_;
    Bisa::SummaryAgg<TGraph> bis7_phi_;

    Bisa::SummaryAgg<TGraph> bis8_eta_;
    Bisa::SummaryAgg<TGraph> bis8_phi_;

    Bisa::SummaryAgg<TGraph> bis7_eta_or_phi_;
    Bisa::SummaryAgg<TGraph> bis7_eta_and_phi_;

    Bisa::SummaryAgg<TGraph> bis8_eta_or_phi_;
    Bisa::SummaryAgg<TGraph> bis8_eta_and_phi_;

    Bisa::SummaryAgg<TGraph> bis78_;
};
