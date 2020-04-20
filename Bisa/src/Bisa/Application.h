#pragma once

#include "Bisa/Core.h"
#include "Bisa/Features/HitCollection.h"
#include "Bisa/DataStream.h"

namespace Bisa {
    
    class BISA_API Application
    {
    public:
        Application();
        virtual ~Application();

        virtual void Run();

        virtual void Step();

    protected:
        Scope<HitCollection> hits_;

    private:
        Scope<DataStream> dataStream_;
        // Scope<FeatureCollection> m_FeatureCollection;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
    
}