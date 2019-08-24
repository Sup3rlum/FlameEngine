#pragma once

#include "nchfx.h"


using namespace std;

enum class DELUSION_DLL ContextProfile
{
	OPENGL_ANY = 0,				//GLFW_OPENGL_CORE_ANY
	OPENGL_CORE = 0x00032001,	//GLFW_OPENGL_CORE_PROFILE
	OPENGL_COMPAT = 0x00032002	//GLFW_OPENGL_COMPAT_PROFILE
};

struct DELUSION_DLL ContextDescription
{
public:
	GLFWwindow* window;
	DELuint32 contextMajorVersion;
	DELuint32 contextMinorVersion;
	DELuint32 contextSampleCount;
	DELuint32 contextCompatibility;
	ContextProfile contextProfile;
	DELuint32 width;
	DELuint32 height;
	bool fullscreen;

	template<size_t _Size>
	void GetGraphicsID(char (&_str)[_Size])
	{
		//strcpy_s(*_str, glGetString(GL_VENDOR));
	}
};