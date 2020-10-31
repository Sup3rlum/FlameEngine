#pragma once

#include "../dll/nchfx.h"


using namespace std;

EXPORT_ENUM ContextProfile
{
	OPENGL_ANY = 0,				//GLFW_OPENGL_CORE_ANY
	OPENGL_CORE = 0x00032001,	//GLFW_OPENGL_CORE_PROFILE
	OPENGL_COMPAT = 0x00032002	//GLFW_OPENGL_COMPAT_PROFILE
};

EXPORT_CLASS ContextDescription
{
public:
	_UNS_ FL_INT32 contextMajorVersion;
	_UNS_ FL_INT32 contextMinorVersion;
	_UNS_ FL_INT32 contextSampleCount;
	_UNS_ FL_INT32 contextCompatibility;
	ContextProfile contextProfile;
	_UNS_ FL_INT32 width;
	_UNS_ FL_INT32 height;
	bool fullscreen;

};