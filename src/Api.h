#pragma once
#ifdef ENGINE_DLL
#undef ENGINE_DLL
#endif

#if defined(_MSC_VER)
#define RENGINE_API __declspec(dllexport)
#elif defined(__GNUC__)
#define RENGINE_API __attribute__((visibility("default")))
#else
#endif

#define RENGINE extern "C" RENGINE_API

#ifdef _DEBUG
#define RENGINE_DEBUG
#endif


#define null nullptr

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#if defined(_MSC_VER)
typedef unsigned long long u64;
#elif defined(__GNUC__)
typedef long unsigned int u64;
#endif
typedef const char string;
typedef int	boolean;

#define RENGINE_TEST_FLAG(a, b) ((u32)a & (u32)b) != 0
