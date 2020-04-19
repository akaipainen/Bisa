#include "bapch.h"
#include "Application.h"

#include "DataStream.h"

namespace Bisa {
    
    Application::Application()
    {
        m_Hits = CreateScope<Hits>();

        auto props = StreamProps("data/ModA04_BIS7_IntMDT_autotrigger_5700V_040320201053.dat");
        m_DataStream = CreateScope<DataStream>(props);
        m_DataStream->SetNewDataCallback([&](Hits& hits) {
            BA_CORE_INFO("Added {0} hits to event", hits.size());
            *m_Hits = hits;
        });
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        for (int i = 0; i < 5; i++)
        {
            auto ok = m_DataStream->FillNextEvent();
            BA_CORE_INFO("Data found: {}", ok);
            for (auto &&h : *m_Hits)
            {
                BA_CORE_INFO("{}", h->ToString());
            }
        }
    }

}
