
#include "OpenGLFRIContext.h"
#include "OpenGLFRIDynamic.h"


#include <GL/wglew.h>



LRESULT CALLBACK OpenGLFRIContext::Win32MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case  WM_KEYDOWN:
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
			break;
		}
		case WM_KEYUP:
		{
			InputHandlerDelegate((FKeyboardKeys)wparam, FKeyboardKeyEvent::OnRelease);
			break;
		}
		default:

			return 	DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	return 0; 
}

void OpenGLFRIContext::Initialize()
{
	if (!GLContextDrvVersion.CheckIsSupportedOnPlatform())
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
	{


		DEVMODE dmScreenSettings;                               // Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));   // Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);       // Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = screenWidth;                // Selected Screen Width
		dmScreenSettings.dmPelsHeight = screenHeight;               // Selected Screen Height
		dmScreenSettings.dmBitsPerPel = 32;                 // Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			printf("Wtf??");
		}

		/*
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(, &mi);
		SetWindowPos(window->win32.handle, HWND_TOPMOST,
			mi.rcMonitor.left,
			mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);*/
	}
	else
	{

		screenWidth = InstanceDescription.Width;
		screenHeight = InstanceDescription.Height;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	win32Context = new Win32Context("Engine2", PosX, PosY, screenWidth, screenHeight, FWin32MessageProcDelegate::Make<OpenGLFRIContext, &OpenGLFRIContext::Win32MessageHandler>(this));
	isActive = true;

	InitializeOpenGL();


	win32Context->Show();


}
bool OpenGLFRIContext::InitializeOpenGLExtensions()
{

	PIXELFORMATDESCRIPTOR pfd;

	Win32Context wglLoadExtContext("WGLContext", 0, 0, 320, 240);
	wglLoadExtContext.SetPixelFormat(&pfd);


	HGLRC g_hRC = wglCreateContext(wglLoadExtContext.hDeviceContext);
	wglMakeCurrent(wglLoadExtContext.hDeviceContext, g_hRC);

	if (g_hRC == NULL)
	{
		printf("HGLRG\n");
	}


	if (glewInit() != GLEW_OK)
	{
		//TODO: Error loading GL extensions
		printf("GLEWINIT ERROR\n");// << std::endl;
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
		printf("PixelFormat error");//
		return false;
	}

	result = SetPixelFormat(win32Context->hDeviceContext, pixelFormat[0], &pixelFormatDescriptor);
	if (result != 1)
	{
		printf("PixelFormat error2\n");//
		return false;
	}

	FStaticArray<int, 5> attributeList =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, (int)GLContextDrvVersion.GetMajor(),
		WGL_CONTEXT_MINOR_VERSION_ARB, (int)GLContextDrvVersion.GetMinor(),
		0
	};

	hGLRenderingContext = wglCreateContextAttribsARB(win32Context->hDeviceContext, 0, attributeList);

	if (hGLRenderingContext == NULL)
	{
		printf("HLGRC2 ERROR\n");// << std::endl;

		return false;
	}

	result = wglMakeCurrent(win32Context->hDeviceContext, hGLRenderingContext);

	wglSwapIntervalEXT(0);


	dynamicAllocator = new OpenGLFRIDynamicAllocator();


	InitializeOpenGLStates();

	InitializeOpenGLUniformExtensions();
}

void OpenGLFRIContext::SwapBuffers()
{
	::SwapBuffers(win32Context->hDeviceContext);
}


void OpenGLFRIContext::HandleEvents()
{
	MSG msg;
	while (PeekMessage(&msg, win32Context->hWindow, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			PollCloseEvent();
		}
		else
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}


OpenGLFRIContext::~OpenGLFRIContext()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hGLRenderingContext);

	delete win32Context;
}



void OpenGLFRIContext::SetCursorPosition(FVector2 pos)
{
	win32Context->SetCursorPosition(pos.x, pos.y);
}


FVector2 OpenGLFRIContext::GetCursorPosition()
{
	int x, y;
	win32Context->GetCursorPosition(&x, &y);


	return FVector2(x, y);
}

IVector2 OpenGLFRIContext::GetViewportSize()
{
	return IVector2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}					


void OpenGLFRIContext::InitializeOpenGLStates()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}