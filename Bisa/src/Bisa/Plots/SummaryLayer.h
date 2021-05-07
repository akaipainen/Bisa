#pragma once
#include "bapch.h"

#include "../Plot.h"
#include "Bisa/Config.h"

#include <TString.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TH1.h>

namespace Bisa {

    template <typename H>
    class SummaryLayer
    {
    public:

        struct DrawProps {
            bool divide;
            bool bis7;
            bool logy;
            bool logz;
            const char* options;
        };

        SummaryLayer(const char* name, const Config& config)
         : name_(name)
         , config_(config)
        {
        }

        ~SummaryLayer()
        {
            for (auto &&layer : layers_)
            {
                delete layer;
            }
        }

        SummaryLayer(const SummaryLayer<H>& other)
         : name_(other.name_)
         , config_(other.config_)
        {
            for (auto &&layer : other.layers_)
            {
                layers_.emplace_back(new H(*layer));
            }
        }

        SummaryLayer<H>& operator=(const SummaryLayer<H>& other)
        {
            auto temp(other);
            std::swap(layers_, temp.layers_);
            std::swap(config_, temp.config_);
            return *this;
        }

        template <typename ... Args>
        void init(Args ... args)
        {
            for (int i = 0; i < 12; i++)
            {
                layers_.emplace_back(
                    new H(
                        Form("%s_%s_%s_layer_%d", name_.c_str(), layer_bis7(i) ? "bis7" : "bis8", layer_eta(i) ? "eta" : "phi", i % 3),
                        Form("%s Layer %d", i % 6 < 3 ? "eta" : "phi", i % 3),
                        std::forward<Args>(args)...
                    )
                );
            }
        }

        unsigned int size() const
        {
            unsigned int sum;
            for (auto &&layer : layers_)
            {
                sum += layer->GetEntries();
            }
            return sum;
        }

        H& operator[](unsigned int i)
        {
            BA_CORE_ASSERT(i < layers_.size(), "Accessing TDC out of bounds.");
            return *layers_[i];
        }

        const H& operator[](unsigned int i) const
        {
            BA_CORE_ASSERT(i < layers_.size(), "Accessing TDC out of bounds.");
            return *layers_[i];
        }

        void for_each(std::function<void(H&)> f)
        {
            for (auto &&layer : layers_)
            {
                f(*layer);
            }
        }

        void setTitles(const std::string& xtitle, const std::string& ytitle)
        {
            for (auto &&layer : layers_)
            {
                layer.GetXaxis()->SetTitle(xtitle);
                layer.GetYaxis()->SetTitle(ytitle);
            }
        }

        void configure()
        {
            // Set histogram maximum
            double max = 0;
            for (auto it = layers_.begin(); it != layers_.end(); it++)
            {   
                if ((*it)->GetMaximum() > max) max = (*it)->GetMaximum();
            }
            for (auto it = layers_.begin(); it != layers_.end(); it++)
            {
                (*it)->SetMaximum(max*1.1);
                (*it)->SetMinimum(0);
            }
        }

        void draw(TCanvas* canvas, DrawProps props=DrawProps())
        {
            if (props.divide) canvas->Divide(2, 3, 0, 0);
            for (unsigned int layer = 0; layer < 12; layer++)
            {
                if (layer_bis7(layer) == props.bis7)
                {
                    // Set correct sub-pad
                    if (layer_eta(layer) && layer_real(layer) == 0) { canvas->cd(1); }
                    if (layer_eta(layer) && layer_real(layer) == 1) { canvas->cd(3); }
                    if (layer_eta(layer) && layer_real(layer) == 2) { canvas->cd(5); }
                    if (!layer_eta(layer) && layer_real(layer) == 0) { canvas->cd(2); }
                    if (!layer_eta(layer) && layer_real(layer) == 1) { canvas->cd(4); }
                    if (!layer_eta(layer) && layer_real(layer) == 2) { canvas->cd(6); }

                    // Configure margins
                    gPad->SetMargin(0.1, 0.1, 0.1, 0.1);

                    if (props.logy)
                    {
                        layers_[layer]->SetMinimum(1);
                        gPad->SetLogy();
                    }
                    if (props.logz) 
                    {
                        layers_[layer]->SetMinimum(1);
                        gPad->SetLogz();
                    }
                    if (!props.logy && !props.logz)
                    {
                        layers_[layer]->SetMinimum(0);
                        gPad->SetLogy(false);
                        gPad->SetLogz(false);
                    }
                    
                    layers_[layer]->Draw(props.options);
                }
            }
        }

        void print(const char* path, DrawProps props = DrawProps())
        {
            TCanvas canvas("canvas", "print canvas", 500, 300);
            props.divide = true;

            if (props.logy)
            {
                gSystem->mkdir(Form("%s/log", path), true);
                gSystem->mkdir(Form("%s/lin", path), true);

                props.logy = false;
                props.bis7 = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin_bis7.pdf", path, name_.c_str()));
                canvas.Clear();
                props.bis7 = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin_bis8.pdf", path, name_.c_str()));
                canvas.Clear();

                props.logy = true;
                props.bis7 = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/log/%s_log_bis7.pdf", path, name_.c_str()));
                canvas.Clear();
                props.bis7 = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/log/%s_log_bis8.pdf", path, name_.c_str()));
                canvas.Clear();
            }

            if (props.logz)
            {
                gSystem->mkdir(Form("%s/log", path), true);
                gSystem->mkdir(Form("%s/lin", path), true);

                props.logz = false;
                props.bis7 = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin_bis7.pdf", path, name_.c_str()));
                canvas.Clear();
                props.bis7 = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin_bis8.pdf", path, name_.c_str()));
                canvas.Clear();

                props.logz = true;
                props.bis7 = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/log/%s_log_bis7.pdf", path, name_.c_str()));
                canvas.Clear();
                props.bis7 = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/log/%s_log_bis8.pdf", path, name_.c_str()));
                canvas.Clear();
            }

            if (!props.logy && !props.logz)
            {
                gSystem->mkdir(Form("%s/lin", path), true);

                props.bis7 = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin_bis7.pdf", path, name_.c_str()));
                canvas.Clear();
                props.bis7 = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin_bis8.pdf", path, name_.c_str()));
                canvas.Clear();
            }
        }

    private:
        bool layer_eta(int i)
        {
            return i % 6 < 3;
        }

        bool layer_bis7(int i)
        {
            return i < 6;
        }

        int layer_real(int i)
        {
            return i % 3;
        }

    private:
        const Config& config_;
        
        std::vector<H*> layers_;
        std::string name_;

    };

}