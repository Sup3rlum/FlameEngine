#pragma once

#include <winuser.h>


#ifdef DELUSIONENGINE_EXPORTS
#define DELUSION_DLL __declspec(dllexport)
#else
#define DELUSION_DLL __declspec(dllimport)
#endif


#define DEL_MSGBOX(a, b) MessageBox(NULL, a, L"DelusionEngine", b)
#define DEL_MSGBOX_ERROR(a) DEL_MSGBOX(a, MB_OK | MB_ICONERROR)

#define EXPORT_CLASS class DELUSION_DLL
#define EXPORT_STRUCT struct DELUSION_DLL
#define EXPORT_ENUM enum EXPORT_CLASS