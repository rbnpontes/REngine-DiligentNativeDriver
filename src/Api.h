#pragma once
#ifdef ENGINE_DLL
#undef ENGINE_DLL
#endif

#define RENGINE_API __declspec(dllexport)

#define RENGINE extern "C" RENGINE_API

#ifdef _DEBUG
#define RENGINE_DEBUG
#endif


#define null nullptr

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulong;
typedef const char string;
typedef int	boolean;

#define RENGINE_TEST_FLAG(a, b) ((uint)a & (uint)b) != 0
