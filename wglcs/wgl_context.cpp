#include "wgl_context.h"


extern "C"
{
	WGLCS_EXPORT HGLRC wglcsCreateGLContextFromHandle(HWND _hWnd, int major, int minor)
	{
		// Pixel Format Specification

		HDC _hdc = GetDC(_hWnd);

		PIXELFORMATDESCRIPTOR pfd =
		{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
		};

		GLint  iPixelFormat;

		if ((iPixelFormat = ChoosePixelFormat(_hdc, &pfd)) == 0)
		{
			return 0;
		}
		if (SetPixelFormat(_hdc, iPixelFormat, &pfd) == FALSE)
		{
			return 0;
		}

		// WGL Context Creation


		GLint attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, major,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor,

			WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		HGLRC _hrc = wglCreateContext(_hdc);
		wglMakeCurrent(_hdc, _hrc);



		glewExperimental = true;

		if (glewInit() != GLEW_OK)
			return 0;


		_hrc = wglCreateContextAttribsARB(_hdc, 0, attribs);
		wglMakeCurrent(_hdc, _hrc);
		
		return _hrc;
	}

	WGLCS_EXPORT void wglcsDeleteContext(HGLRC _hrc)
	{
		wglDeleteContext(_hrc);
	}
	WGLCS_EXPORT void wglcsSwapBuffers(HDC _hdc)
	{
		SwapBuffers(_hdc);
	}
	WGLCS_EXPORT void wglcsClear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}