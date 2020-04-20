#include "bapch.h"
#include "Application.h"

#include "DataStream.h"

namespace Bisa {
    
    Application::Application()
    {
        hits_ = CreateScope<HitCollection>();

        auto props = StreamProps("data/ModA04_BIS7_IntMDT_autotrigger_5700V_040320201053.dat");
        dataStream_ = CreateScope<DataStream>(props);
        dataStream_->SetNewDataCallback([&](HitCollection& hits) {
            BA_CORE_INFO("Added {0} hits to event", hits.size());
            *hits_ = hits;
        });
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        for (int i = 0; i < 5; i++)
        {
            auto ok = dataStream_->FillNextEvent();
            BA_CORE_INFO("Data found: {}", ok);
            for (auto &&h : *hits_)
            {
                BA_CORE_INFO("{}", h.second->toString());
            }
            
            Step();
        }
    }

    void Application::Step()
    {

    }

}
