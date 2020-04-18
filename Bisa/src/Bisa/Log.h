#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Bisa {

    class BISA_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

// Core log macros
#define BA_CORE_TRACE(...)   ::Bisa::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BA_CORE_INFO(...)    ::Bisa::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BA_CORE_WARN(...)    ::Bisa::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BA_CORE_ERROR(...)   ::Bisa::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BA_CORE_FATAL(...)   ::Bisa::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define BA_TRACE(...)        ::Bisa::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BA_INFO(...)         ::Bisa::Log::GetClientLogger()->info(__VA_ARGS__)
#define BA_WARN(...)         ::Bisa::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BA_ERROR(...)        ::Bisa::Log::GetClientLogger()->error(__VA_ARGS__)
#define BA_FATAL(...)        ::Bisa::Log::GetClientLogger()->fatal(__VA_ARGS__)
