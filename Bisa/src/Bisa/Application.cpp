#include "Application.h"

#include <TStyle.h>
#include <TSystem.h>
#include <TError.h>

namespace Bisa {
    
    Application::Application(const Config& config)
     : config_(config)
    {
        dataStream_ = make_unique<DataStream>(config_);
        dataStream_->set_new_data_callback([&](HitCollection& hits) {
            hits_ = std::move(hits);
        });

        gSystem->mkdir(config_.output_path().c_str(), true);
        file_ = make_unique<TFile>(Form("%s/output.root", config_.output_path().c_str()), "recreate");
        tree_ = make_unique<TTree>("tree", "TDC Histograms");

        gStyle->SetHistMinimumZero();
        gErrorIgnoreLevel = kWarning;
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        for (int i = 0; i < 5; i++)
        {
            auto ok = dataStream_->fill_next_event();
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
