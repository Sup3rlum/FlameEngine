#pragma once

#include <winuser.h>
#include <stdio.h>
#include <functional>


#ifdef FLAMEENGINE_EXPORTS
#define FLAME_DLL __declspec(dllexport)
#else
#define FLAME_DLL __declspec(dllimport)
#endif


#define FLAME_MSGBOX(a, b) MessageBox(NULL, a, L"FlameEngine", b)
#define FLAME_MSGBOX_ERROR(a) FLAME_MSGBOX(a, MB_OK | MB_ICONERROR)

#define EXPORT_CLASS	class FLAME_DLL
#define EXPORT_STRUCT	struct FLAME_DLL
#define EXPORT_ENUM		enum EXPORT_CLASS

#define STRING std::string


#define FUNC_POINTER(_returnType, _args) std::function<_returnType(_args)>


#define STRINGIFY(A) #A


#define FL_INT32 int
#define FL_INT64 long long

#define _UNS_ unsigned

#define FL_HANDLE _UNS_ FL_INT32


