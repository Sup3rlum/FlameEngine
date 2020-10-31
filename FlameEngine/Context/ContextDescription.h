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
	DELuint32 contextMajorVersion;
	DELuint32 contextMinorVersion;
	DELuint32 contextSampleCount;
	DELuint32 contextCompatibility;
	ContextProfile contextProfile;
	DELuint32 width;
	DELuint32 height;
	bool fullscreen;

};