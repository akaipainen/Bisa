#pragma once

#include <TTree.h>
#include "nlohmann/json.hpp"
#include "Bisa/Config.h"

using json = nlohmann::json;

extern Bisa::Application* Bisa::CreateApplication(const char *datafile, const char *output_path);

int main(int argc, char** argv)
{
    Bisa::Log::Init();
    BA_CORE_INFO("Initialized Log!");

    if (argc != 4) 
    {
        BA_CORE_ERROR("Usage: ./Commissioning <path to config> <path to data> <output path>");
        return 0;
    }

    // initialize global config
    Bisa::config.init(argv[1]);
    BA_CORE_INFO("Config initialized: {}", argv[1]);

    auto app = Bisa::CreateApplication(argv[2], argv[3]);
    app->Run();
    delete app;
}
