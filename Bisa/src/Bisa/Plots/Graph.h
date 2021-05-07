#pragma once
#include "bapch.h"

#include "../Plot.h"
#include "Bisa/Config.h"

#include <TString.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TGraph.h>

namespace Bisa {

    class Graph
    {
    public:

        struct DrawProps {
            bool logy;
            bool logz;
            const char* options;
        };

        Graph(const char* name, const Config& config)
         : name_(name)
         , config_(config)
        {
        }

        ~Graph()
        {
            delete graph_;
        }

        Graph(const TGraph& other)
         : name_(other.name_)
         , config_(other.config_)
        {
            graph_ = new TGraph(other->graph_);
        }

        TGraph& operator=(const TGraph& other)
        {
            auto temp(other);
            std::swap(graph_, temp.graph_);
            std::swap(config_, temp.config_);
            return *this;
        }

        template <typename ... Args>
        void init(Args ... args)
        {
            graph_ = new TGraph(std::forward<Args>(args)...);
        }

        TGraph& operator()()
        {
            return *graph_;
        }

        void apply(std::function<void(H&)> f)
        {
            f(*graph_);
        }

        void setTitles(const std::string& xtitle, const std::string& ytitle)
        {
            graph_->GetXaxis()->SetTitle(xtitle);
            graph_->GetYaxis()->SetTitle(ytitle);
        }

        void draw(TCanvas* canvas, DrawProps props=DrawProps())
        {
            if (props.logy) 
            {
                graph_->SetMinimum(1);
                gPad->SetLogy();
            }
            if (props.logz) 
            {
                graph_->SetMinimum(1);
                gPad->SetLogz();
            }
            if (!props.logy && !props.logz)
            {
                graph_->SetMinimum(0);
                gPad->SetLogy(false);
                gPad->SetLogz(false);
            }
            
            graph_->Draw(props.options);
        }

        void print(const char* path, DrawProps props = DrawProps())
        {
            TCanvas canvas("canvas", "print canvas", 500, 300);

            if (props.logy)
            {
                gSystem->mkdir(Form("%s/log", path), true);
                gSystem->mkdir(Form("%s/lin", path), true);

                props.logy = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin.pdf", path, name_.c_str()));
                canvas.Clear();

                props.logy = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/log/%s_log.pdf", path, name_.c_str()));
                canvas.Clear();
            }

            if (props.logz)
            {
                gSystem->mkdir(Form("%s/log", path), true);
                gSystem->mkdir(Form("%s/lin", path), true);

                props.logz = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin.pdf", path, name_.c_str()));
                canvas.Clear();

                props.logz = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/log/%s_log.pdf", path, name_.c_str()));
                canvas.Clear();
            }

            if (!props.logy && !props.logz)
            {
                gSystem->mkdir(Form("%s/lin", path), true);

                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin.pdf", path, name_.c_str()));
                canvas.Clear();
            }
        }

    private:
        const Config& config_;
        
        TGraph* graph_;
        std::string name_;

    };

}