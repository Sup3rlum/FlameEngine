#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>


#ifdef WGLCS_EXPORTS
#define WGLCS_EXPORT __declspec(dllexport)
#else
#define WGLCS_EXPORT __declspec(dllimport)
#endif