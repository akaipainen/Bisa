#pragma once

#include <TTree.h>
#include "nlohmann/json.hpp"
#include "Bisa/Config.h"

#ifdef BA_PLATFORM_MACOS

using json = nlohmann::json;

extern Bisa::Application* Bisa::CreateApplication(const Config& config);

int main(int argc, char** argv)
{
    Bisa::Log::Init();
    BA_CORE_INFO("Initialized Log!");

    if (argc != 2) 
    {
        BA_CORE_ERROR("Usage: ./Commissioning <path to config>");
        return 0;
    }

    Bisa::Config config(argv[1]);

    auto app = Bisa::CreateApplication(config);
    app->Run();
    delete app;
}

#endif