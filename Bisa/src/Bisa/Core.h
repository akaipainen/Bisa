#pragma once

#include <memory>

// Platform detection using predefined macros
#if defined _WIN32
    #if defined _WIN64
        #define BA_PLATFORM_WINDOWS
    #else
        #error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define BA_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define BA_PLATFORM_MACOS
    #else
        #error "Unknown Apple platform!"
    #endif
#elif defined (__ANDROID__)
    #define BA_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define BA_PLATFORM_LINUX
#endif

// Dynamic Link Library
#if defined BA_PLATFORM_WINDOWS
    #if defined BA_DYNAMIC_LINK
        #if defined BA_BUILD_DLL
            #define BISA_API __declspec(dllexport)
        #else
            #define BISA_API __declspec(dllimport)
        #endif
    #else
        #define BISA_API
    #endif
#elif defined BA_PLATFORM_MACOS || defined BA_PLATFORM_LINUX
    #if defined BA_DYNAMIC_LINK
        #if defined BA_BUILD_DLL
            #define BISA_API __attribute__((visibility("default")))
        #else
            #define BISA_API
        #endif
    #else
        #define BISA_API
    #endif
#else
    #error "Unsupported platform!"
#endif

#if defined BA_DEBUG
    #define BA_ENABLE_ASSERTS
#endif

#ifdef BA_ENABLE_ASSERTS
    #if defined BA_PLATFORM_WINDOWS
        #define BA_ASSERT(x, ...) { if(!(x)) { BA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
        #define BA_CORE_ASSERT(x, ...) { if(!(x)) { BA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #else
        #define BA_ASSERT(x, ...) { if(!(x)) { BA_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
        #define BA_CORE_ASSERT(x, ...) { if(!(x)) { BA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); } }
    #endif
#else
	#define BA_ASSERT(x, ...)
	#define BA_CORE_ASSERT(x, ...)
#endif

template <typename T, typename ... Args>
constexpr std::unique_ptr<T> make_unique(Args&& ... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

namespace Bisa {
    const unsigned int CHANNEL_STRIP_MAPPING[32] = {0,  4,  8, 12, 16, 20, 24, 28, 
                                                    1,  5,  9, 13, 17, 21, 25, 29, 
                                                    2,  6, 10, 14, 18, 22, 26, 30, 
                                                    3,  7, 11, 15, 19, 23, 27, 31};

    const double STRIP_WIDTH_CM = 2.5;
    const double BIS7_ETA_STRIPS = 32;
    const double BIS7_PHI_STRIPS = 64;
    const double BIS8_ETA_STRIPS = 16;
    const double BIS8_PHI_STRIPS = 64;

    const double BC_RESOLUTION_NS = 25;
    const double FINE_TIME_RESOLUTION_NS = 0.1953125;
};