#pragma once

#include "Bisa/Core.h"
#include "Bisa/Features/Hits.h"

namespace Bisa {
    
    class BISA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    private:
        Scope<Hits> m_Hits;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
    
}