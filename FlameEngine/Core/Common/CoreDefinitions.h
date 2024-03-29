#pragma once

#include <windows.h>
#include <stdio.h>
#include <functional>




#ifdef FLAME_API_EXPORTS
#define FLAME_API __declspec(dllexport)
#else
#define FLAME_API __declspec(dllimport)
#endif

/*
*	Structural Macros
*
* 
*/



#define EXPORT_FORWARD_DECL(type, name)		type FLAME_API name
#define EXPORT(type, name)					type FLAME_API name 



#define FLAME_MSGBOX(a, b) MessageBox(NULL, a, L"FlameEngine", b)
#define FLAME_MSGBOX_ERROR(a) FLAME_MSGBOX(a, MB_OK | MB_ICONERROR)




typedef uint32_t uint32;
typedef uint64_t uint64;


typedef int32_t int32;
typedef int64_t int64;


typedef unsigned char byte;
typedef char		  sbyte;



EXPORT(enum class, FResult)
{
	FAIL,
	SUCCESS,
	UNKNOWN
};