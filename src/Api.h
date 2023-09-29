#pragma once
#ifdef ENGINE_DLL
#undef ENGINE_DLL
#endif

#define RENGINE_API __declspec(dllexport)

#define RENGINE extern "C" RENGINE_API

#define null nullptr

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef const char string;
typedef int	boolean;
