#include "Application.h"

#include <TStyle.h>
#include <TSystem.h>
#include <TError.h>

namespace Bisa {
    
    Application::Application(const char *datafile, const char *output_path)
     : data_stream_(datafile)
     , output_path_(output_path)
    {
        data_stream_.set_new_data_callback([&](HitCollection& hits) {
            hits_ = std::move(hits);
        });

        gSystem->mkdir(output_path, true);
        file_ = new TFile(Form("%s/output.root", output_path), "recreate");
        tree_ = new TTree("tree", "TDC Histograms");

        gStyle->SetHistMinimumZero();
        gErrorIgnoreLevel = kWarning;
    }

    Application::~Application()
    {
        delete tree_;
        delete file_;
    }

    void Application::Run()
    {
        for (int i = 0; i < 5; i++)
        {
            auto ok = data_stream_.fill_next_event();
            BA_CORE_INFO("Data found: {}", ok);
            for (auto &&h : hits_)
            {
                BA_CORE_INFO("{}", h.to_string());
            }

            Step();
        }
    }

    void Application::Step()
    {

    }

}
