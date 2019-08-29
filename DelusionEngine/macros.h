#pragma once

#include <winuser.h>
#include <stdio.h>
#include <functional>


#ifdef DELUSIONENGINE_EXPORTS
#define DELUSION_DLL __declspec(dllexport)
#else
#define DELUSION_DLL __declspec(dllimport)
#endif


#define DEL_MSGBOX(a, b) MessageBox(NULL, a, L"DelusionEngine", b)
#define DEL_MSGBOX_ERROR(a) DEL_MSGBOX(a, MB_OK | MB_ICONERROR)

#define EXPORT_CLASS	class DELUSION_DLL
#define EXPORT_STRUCT	struct DELUSION_DLL
#define EXPORT_ENUM		enum EXPORT_CLASS

#define STRING string


#define FUNC_POINTER(_returnType, _args) std::function<_returnType(_args)>