#pragma once

#include "nchfx.h"

struct DELUSION_DLL ContextParameters
{
public:
	GLuint contextMajorVersion;
	GLuint contextMinorVersion;
	GLuint contextSampleCount;
	GLuint contextCompatibility;
	GLuint contextProfile;
	GLuint width;
	GLuint height;
};