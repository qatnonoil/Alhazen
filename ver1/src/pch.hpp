﻿#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#define NOMINMAX

#if _MSC_VER && !__INTEL_COMPILER
#define WINDOWS
#endif

#if defined(__clang__)
#define CLANG
#endif

// forceinline
#if defined(WINDOWS)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE __attribute__((always_inline))
#endif

// C
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <ctime>
#include <cinttypes>
#include <cstdarg>
#define _USE_MATH_DEFINES
#include <cmath>

// C++
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <functional>
#include <random>
#include <array>
#include <valarray>
#include <unordered_map>
#include <deque>
#include <sstream>
#include <iomanip>
#include <random>
#include <thread>
#include <typeindex>
#include <shared_mutex>
#include <atomic>

// SIMD
#if defined(CLANG)
#include <x86intrin.h>
#else
#include <mmintrin.h>
#include <intrin.h>
#endif

#if defined(WINDOWS)
// portableだけれども各種SDKに依存するヘッダ
#include <direct.h>
// TBB内にwindows.hがあり、これよりも先にwinsock2.hがないとビルドエラーになってしまう
#include <winsock2.h>
#else
#include <sys/stat.h> // for mkdir()
#endif

// Third party
#include "tinyxml2.h"
#include <tbb/tbb.h>

// macro
#define PI float(M_PI)
#define INV_PI float(M_1_PI)

//
#if defined(WINDOWS)
#define OPT_ON __pragma(optimize("", on))
#define OPT_OFF __pragma(optimize("", off))
#else
#define OPT_OFF _Pragma("clang optimize off")
#define OPT_ON _Pragma("clang optimize on")
#endif

//
#define DEG2RAD(deg) ((deg) / 180.0f * PI)

// 波長
#define WAVE_LENGTH_D 0.58930f
#define WAVE_LENGTH_d 0.58756f
#define WAVE_LENGTH_F 0.4861f
#define WAVE_LENGTH_C 0.6563f
#define WAVE_LENGTH_e 0.5461f
#define WAVE_LENGTH_F_DASH 0.4880f
#define WAVE_LENGTH_C_DASH 0.6439f

// Debug関連
#if defined(WINDOWS)
#if defined(__clang__)
#define DBG_BREAK() __debugbreak()
#else
#define DBG_BREAK()                                                            \
    __ud2();                                                                   \
    __debugbreak()
#endif
#else
#define DBG_BREAK() __builtin_trap()
#endif

#if defined(NO_ASSERT)
#define AL_ASSERT_DEBUG(expr)
#define AL_VALID(expr) expr
#else
#define AL_ASSERT_DEBUG(expr)                                                  \
    if (!(expr)) {                                                             \
        printf("\n%s %s %d\n", #expr, __FILE__, __LINE__);                     \
        DBG_BREAK();                                                           \
    }
#define AL_VALID(expr) AL_ASSERT_DEBUG(expr)
#endif
#define AL_ASSERT_ALWAYS(expr)                                                 \
    if (!(expr)) {                                                             \
        printf("\n%s %s %d\n", #expr, __FILE__, __LINE__);                     \
        DBG_BREAK();                                                           \
    }

// aligin
#if defined(WINDOWS)
#define AL_ALIGN(bit) __declspec(align(bit))
#else
#define AL_ALIGN(bit) alignas(bit)
#endif
#define ALIGN16 AL_ALIGN(16)
#define ALIGN32 AL_ALIGN(32)
#define ALIGN64 AL_ALIGN(64)
#define ALIGN128 AL_ALIGN(128)

// inline
#if defined(WINDOWS)
#define AL_FORCEINLINE __forceinline
#define INLINE inline
#else
#define AL_FORCEINLINE inline
#define INLINE inline
#endif

//
#pragma warning(disable : 4201) // 無名の構造体または共用体
#pragma warning(disable : 4324) // 構造体がパッドされた
#pragma warning(disable : 4127) // 条件式が定数
#pragma warning(disable : 4503) // chaiscript対策

// TLS
#if defined(WINDOWS)
#define AL_TLS __declspec(thread)
#else
#define AL_TLS __thread
#endif

#define AL_FINAL final

// 全てのclassのリスト
#include "fwd.hpp"
