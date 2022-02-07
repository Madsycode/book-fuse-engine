#pragma once

// std libs
#include <set>
#include <random>
#include <limits>
#include <vector>
#include <string>
#include <bitset>
#include <cassert>
#include <memory>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>

// SDL2 libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

// export api
#ifdef FUSE_EXPORT
    #ifdef _MSC_VER
        #define FUSE_API __declspec(dllexport)
    #else
        #define FUSE_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define FUSE_API __declspec(dllimport)
    #else
        #define FUSE_API
    #endif
#endif

// logging macros
#if defined(_DEBUG)
	#define FUSE_LOG(level, message, ...) \
        fprintf(stderr, "[%s] ", level); \
        fprintf(stderr, message, ##__VA_ARGS__); \
        fprintf(stderr, "\n");

	#define FUSE_WARNING(message, ...) FUSE_LOG("warning", message, ##__VA_ARGS__)
	#define FUSE_ERROR(message, ...) FUSE_LOG("error", message, ##__VA_ARGS__)
	#define FUSE_INFO(message, ...) FUSE_LOG("info", message, ##__VA_ARGS__)
#else
	#define FUSE_INFO
	#define FUSE_ERROR	
	#define FUSE_WARNING
#endif

// runtime assertion
#define FUSE_ASSERT assert

// static assertion
#if defined(__clang__) || defined(__gcc__)
    #define FUSE_STATIC_ASSERT _Static_assert
#else
    #define FUSE_STATIC_ASSERT static_assert
#endif

// function inlining
#if defined(__clang__) || defined(__gcc__)
    #define FUSE_INLINE __attribute__((always_inline)) inline
    #define FUSE_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
    #define FUSE_INLINE __forceinline
    #define FUSE_NOINLINE __declspec(noinline)
#else
    #define FUSE_INLINE inline
    #define FUSE_NOINLINE
#endif

// free allocated memory and sets it to null pointer
#define FUSE_DELETE(m) if (m != NULL) { delete (m); m = NULL; }

// attach lamda function to function call
#define FUSE_BIND(f) [this](auto&&... args)->decltype(auto) \
{ return this->f(std::forward<decltype(args)>(args)...); }

// get current time in second and millisecond
#define get_ticks() SDL_GetTicks()/1000.0f
#define get_ticks_ms() SDL_GetTicks()

// window size
#define INVALID_ID 0
#define MAX_DELTATIME 0.05f

// universal unique id (uuid)
namespace fuse {
	using uuid64 = size_t;
	FUSE_API uuid64 generate_uuid();
}
