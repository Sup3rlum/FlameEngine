#include "CustomContext.h"

void CustomContext::AttachToHandle(HWND _hWnd)
{
	_handle = _hWnd;
}

void CustomContext::Initialize(ContextDescription* _cont)
{
	_hdc = GetDC(_handle);

	CustomSetPixelFormat(_hdc);

	GLint attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,

		WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	_hrc = wglCreateContext(_hdc);
	
	wglMakeCurrent(_hdc, _hrc);


	// GLEW
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
		DEL_MSGBOX_ERROR(L"Failed to initialize GLEW");


	_hrc = wglCreateContextAttribsARB(_hdc, 0, attribs);

	wglMakeCurrent(_hdc, _hrc);



}
DELuint32 CustomContext::CustomSetPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
		1,                                // version number 
		PFD_DRAW_TO_WINDOW |              // support window 
		PFD_SUPPORT_OPENGL |              // support OpenGL 
		PFD_DOUBLEBUFFER,                 // double buffered 
		PFD_TYPE_RGBA,                    // RGBA type 
		24,                               // 24-bit color depth 
		0, 0, 0, 0, 0, 0,                 // color bits ignored 
		0,                                // no alpha buffer 
		0,                                // shift bit ignored 
		0,                                // no accumulation buffer 
		0, 0, 0, 0,                       // accum bits ignored 
		32,                               // 32-bit z-buffer     
		0,                                // no stencil buffer 
		0,                                // no auxiliary buffer 
		PFD_MAIN_PLANE,                   // main layer 
		0,                                // reserved 
		0, 0, 0                           // layer masks ignored 
	};

	GLint  iPixelFormat;

	// get the device context's best, available pixel format match 
	if ((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		DEL_MSGBOX_ERROR(L"ChoosePixelFormat Failed");
		return 0;
	}

	// make that match the device context's current pixel format 
	if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		DEL_MSGBOX_ERROR(L"SetPixelFormat Failed");
		return 0;
	}

	return 1;
}
void CustomContext::SwapChain()
{
	SwapBuffers(_hdc);
}
void CustomContext::Destroy()
{
	wglDeleteContext(_hrc);
}
void CustomContext::GetCursorPosition(double* x, double* y)
{

}
void CustomContext::SetCursorPosition(double x, double y)
{

}
KeyState CustomContext::GetKeyState(Keys keys)
{
	return KeyState::RELEASED;
}