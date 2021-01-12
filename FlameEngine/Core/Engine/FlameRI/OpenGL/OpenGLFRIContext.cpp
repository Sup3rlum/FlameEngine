
#include "OpenGLFRIContext.h"



#include <GL/wglew.h>



LRESULT OpenGLFRIContext::Win32MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (umessage == WM_KEYDOWN)
	{

		uint32 repeatCount = wparam & 0x7FFF;

		if (repeatCount > 0)
		{
			InputHandlerDelegate((FKeyboardKeys)wparam, FKeyboardKeyEvent::OnHold);
		}
		else
		{
			InputHandlerDelegate((FKeyboardKeys)wparam, FKeyboardKeyEvent::OnPress);
		}
	}
	if (umessage == WM_KEYUP)
	{
		InputHandlerDelegate((FKeyboardKeys)wparam, FKeyboardKeyEvent::OnRelease);
	}
}

void OpenGLFRIContext::Initialize()
{
	if (!GLContextDrvVersion->CheckIsSupportedOnPlatform())
	{
		return;
	}

	if (!InitializeOpenGLExtensions())
	{
		return;
	}


	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int PosX = 100, PosY = 100;

	if (InstanceDescription.IsFullscreen)
	{/*
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		PosX = PosY = 0;*/
	}
	else
	{

		screenWidth = InstanceDescription.Width;
		screenHeight = InstanceDescription.Height;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	win32Context = new Win32Context("Engine", PosX, PosY, screenWidth, screenHeight, FWin32MessageProcDelegate::Make<OpenGLFRIContext, &Win32MessageHandler>(this));

	InitializeOpenGL();

	win32Context->Show();

}
bool OpenGLFRIContext::InitializeOpenGLExtensions()
{

	PIXELFORMATDESCRIPTOR pfd;

	Win32Context wglLoadExtContext = Win32Context("WGLContext", 0, 0, 320, 240);
	wglLoadExtContext.SetPixelFormat(&pfd);


	HGLRC g_hRC = wglCreateContext(wglLoadExtContext.hDeviceContext);
	wglMakeCurrent(wglLoadExtContext.hDeviceContext, g_hRC);

	if (g_hRC == NULL)
	{
		//std::cout << "HGLRC ERROR" << std::endl;
	}


	if (glewInit() != GLEW_OK)
	{
		//TODO: Error loading GL extensions
		//	std::cout << "GLEWINIT ERROR" << std::endl;
	}


	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(g_hRC);


	return true;
}
bool OpenGLFRIContext::InitializeOpenGL()
{
	int pixelFormat[1];
	unsigned int formatCount;
	int result;
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;



	FStaticArray<int, 23> attributeListInt =
	{
		WGL_SUPPORT_OPENGL_ARB,		TRUE,
		WGL_DRAW_TO_WINDOW_ARB,		TRUE,
		WGL_ACCELERATION_ARB,		WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,			24,
		WGL_DEPTH_BITS_ARB,			24,
		WGL_DOUBLE_BUFFER_ARB,		TRUE,
		WGL_SWAP_METHOD_ARB,		WGL_SWAP_EXCHANGE_ARB, // Double Buffered
		WGL_PIXEL_TYPE_ARB,			WGL_TYPE_RGBA_ARB,
		WGL_STENCIL_BITS_ARB,		8,
		WGL_SAMPLE_BUFFERS_ARB,		(int)InstanceDescription.SampleCount > 0 ? GL_TRUE : GL_FALSE,
		WGL_SAMPLES_ARB,			(int)InstanceDescription.SampleCount,
		0
	};

	result = wglChoosePixelFormatARB(win32Context->hDeviceContext, attributeListInt, NULL, 1, pixelFormat, &formatCount);

	if (result != 1)
	{
		//std::cout << "PixelFormat error" << std::endl;
		return false;
	}

	result = SetPixelFormat(win32Context->hDeviceContext, pixelFormat[0], &pixelFormatDescriptor);
	if (result != 1)
	{
		return false;
	}

	FStaticArray<int, 5> attributeList =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, (int)GLContextDrvVersion->GetMajor(),
		WGL_CONTEXT_MINOR_VERSION_ARB, (int)GLContextDrvVersion->GetMinor(),
		0
	};

	hGLRenderingContext = wglCreateContextAttribsARB(win32Context->hDeviceContext, 0, attributeList);

	if (hGLRenderingContext == NULL)
	{
		//std::cout << "HLGRC2 ERROR" << std::endl;

		return false;
	}

	result = wglMakeCurrent(win32Context->hDeviceContext, hGLRenderingContext);


}

void OpenGLFRIContext::SwapBuffers()
{
	::SwapBuffers(win32Context->hDeviceContext);
}


bool OpenGLFRIContext::HandleEvents()
{
	MSG msg;
	while (PeekMessage(&msg, win32Context->hWindow, 0, 0, PM_NOREMOVE))
	{
		if (GetMessage(&msg, win32Context->hWindow, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			return false;
		}
	}
	return true;
}


OpenGLFRIContext::~OpenGLFRIContext()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hGLRenderingContext);

	delete win32Context;
}