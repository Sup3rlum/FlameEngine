#pragma once

#include "../FRIContext.h"
#include "OpenGLFRIContextVersion.h"
#include "OpenGL.h"
#include "Platform/Windows/Win32Context.h"

class OpenGLFRIContext : public FRIContext
{
public:
	OpenGLFRIContext(FRIRenderingContextDescription description, FRIContext* contextToCopy = NULL, OpenGLContextDrvVersion* glVer = new FOpenGLVersion_45()) :
		FRIContext(description, contextToCopy),
		GLContextDrvVersion(glVer)
	{

	}


	~OpenGLFRIContext();

	void Run();
	void Initialize();
	bool InitializeOpenGLExtensions();
	bool InitializeOpenGL();
	void* GetPlatformSpecificHandle() { return win32Context; }

	void SwapBuffers();

	bool HandleEvents();

	LRESULT Win32MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

private:


	HGLRC hGLRenderingContext;

	OpenGLContextDrvVersion* GLContextDrvVersion;
	Win32Context* win32Context;


};