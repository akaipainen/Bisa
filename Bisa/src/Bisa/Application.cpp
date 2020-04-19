#include "bapch.h"
#include "Application.h"

#include "DataStream.h"

namespace Bisa {
    
    Application::Application()
    {
        m_Hits = std::unique_ptr<Hits>(new Hits());

        auto props = StreamProps("data/ModA04_BIS7_IntMDT_autotrigger_5700V_040320201053.dat");
        DataStream ds(props);
        ds.SetNewDataCallback([&](Hits& hits) {
            BA_CORE_INFO("Added {0} hits to event", hits.size());
            *m_Hits = hits;
        });
        auto ok = ds.FillNextEvent();
        BA_CORE_INFO("Data found: {}", ok);
        for (auto &&h : *m_Hits)
        {
            BA_CORE_INFO("{}", h->ToString());
        }
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (true);
    }

}
