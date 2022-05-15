#pragma once

#include "Platform/Windows/Win32Context.h"
#include "../FRIContext.h"
#include "OpenGLFRIContextVersion.h"
#include "OpenGL.h"


class OpenGLFRIContext : public FRIContext
{
public:
	OpenGLFRIContext(FRIRenderingContextDescription description, FRIContext* contextToCopy = NULL, OpenGLContextDrvVersion glVer = FOpenGLVersion_45()) :
		FRIContext(description, contextToCopy),
		GLContextDrvVersion(glVer),
		win32Context(description.WinContext),
		hGLRenderingContext(NULL)
	{
		
	}


	~OpenGLFRIContext();

	void Initialize();
	bool InitializeOpenGLExtensions();
	bool InitializeOpenGLUniformExtensions();
	bool InitializeOpenGL();
	void InitializeOpenGLStates();
	void* GetPlatformSpecificHandle() { return win32Context; }
	void SwapBuffers();

	void ShowCursor();
	void HideCursor();

	void HandleEvents();

	FVector2 GetCursorPosition();
	void SetCursorPosition(FVector2 pos);

	FViewportRect GetViewport();


	LRESULT Win32MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

private:

	HGLRC hGLRenderingContext;

	OpenGLContextDrvVersion GLContextDrvVersion;
	Win32Context* win32Context;

};