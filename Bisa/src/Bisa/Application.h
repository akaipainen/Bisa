#pragma once

#include "Bisa/Core.h"
#include "Bisa/Config.h"
#include "Bisa/Features/HitCollection.h"
#include "Bisa/Data/DataStream.h"

namespace Bisa {
    
    class BISA_API Application
    {
    public:
        Application(const Config& config);
        virtual ~Application();

        virtual void Run();

        virtual void Step();

    protected:
        Scope<HitCollection> hits_;
        Scope<DataStream> dataStream_;

        Config config_;
    };

    // To be defined in CLIENT
    Application* CreateApplication(const Config& config);
    
}