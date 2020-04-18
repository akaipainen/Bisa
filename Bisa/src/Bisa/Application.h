#pragma once

#include "Core.h"

namespace Bisa {
    
    class BISA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    // To be defined in CLIENT
    Application* CreateApplication();
    
}