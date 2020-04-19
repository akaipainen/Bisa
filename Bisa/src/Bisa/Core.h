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
    #error "Linux is not supported!"
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

namespace Bisa {
    template <typename T>
    using Scope = std::unique_ptr<T>;
    // template <typename T, typename ... Args>
    // constexpr Scope<T> CreateScope(Args&& ... args)
    // {
    //     return std::make_unique<T>(std::forward<Args>(args)...);
    // }

    template<typename T>
	using Ref = std::shared_ptr<T>;
	// template<typename T, typename ... Args>
	// constexpr Ref<T> CreateRef(Args&& ... args)
	// {
	// 	return std::make_shared<T>(std::forward<Args>(args)...);
	// }
}