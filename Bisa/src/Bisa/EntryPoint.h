#pragma once

#include <TTree.h>
#include "nlohmann/json.hpp"

#ifdef BA_PLATFORM_MACOS

using json = nlohmann::json;

extern Bisa::Application* Bisa::CreateApplication();

int main(int argc, char** argv)
{
    json j;
    j["pi"] = 3.1415926;

    Bisa::Log::Init();
    BA_CORE_WARN("Initialized Log!");
    BA_INFO("Hello! Var={0}", j["pi"].get<double>());

    TTree tree("tree", "Tree");

    auto app = Bisa::CreateApplication();
    app->Run();
    delete app;
}

#endif