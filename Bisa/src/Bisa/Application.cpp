#include "Application.h"

#include <TStyle.h>
#include <TError.h>

namespace Bisa {
    
    Application::Application(const Config& config)
     : config_(config)
    {
        hits_ = CreateScope<HitCollection>();

        dataStream_ = CreateScope<DataStream>(config_);
        dataStream_->set_new_data_callback([&](HitCollection& hits) {
            *hits_ = hits;
        });

        file_ = CreateScope<TFile>("output/output.root", "recreate");
        tree_ = CreateScope<TTree>("tree", "TDC Histograms");

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
            for (auto &&h : *hits_)
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
