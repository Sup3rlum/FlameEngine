#include "Win32Context.h"



LRESULT CALLBACK Win32Context::Win32MessageProcSignature(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{

	Win32Context* pInstance;
    if (umessage == WM_NCCREATE)
    {
        pInstance = static_cast<Win32Context*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pInstance)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    }
    else
    {
        pInstance = reinterpret_cast<Win32Context*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }


	if (pInstance)
	{
		return pInstance->MessageHandler.Handler(hwnd, umessage, wparam, lparam);
	}


	return DefWindowProc(hwnd, umessage, wparam, lparam);
}

Win32MessageHandler::Win32MessageHandler(FWin32MessageProcDelegate del) : Handler(del)
{
}

DefaultWin32MessageHandler::DefaultWin32MessageHandler() :
	Win32MessageHandler([](HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		})
{
}


Win32Context::Win32Context(FString Name, int32 PosX, int32 PosY, int32 Width, int32 Height, bool fullscreen, Win32MessageHandler handler) :
	Name(Name),
	PosX(PosX),
	PosY(PosY),
	Width(Width),
	Height(Height),
	MessageHandler(handler)
{

	if (fullscreen)
	{
		DEVMODE dmScreenSettings;                               // Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));   // Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);       // Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = Width;                // Selected Screen Width
		dmScreenSettings.dmPelsHeight = Height;               // Selected Screen Height
		dmScreenSettings.dmBitsPerPel = 32;                 // Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			printf("Wtf??");
		}

		
		/*MONITORINFO mi = {sizeof(mi)};
		GetMonitorInfo(, & mi);
		SetWindowPos(window->win32.handle, HWND_TOPMOST,
			mi.rcMonitor.left,
			mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);*/
	}

	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = Win32MessageProcSignature;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = Name.ToPlatformString();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);



	hWindow = CreateWindowEx(
		WS_EX_APPWINDOW,
		Name.ToPlatformString(),
		Name.ToPlatformString(),
		WS_OVERLAPPEDWINDOW,
		PosX,
		PosY,
		Width,
		Height,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this);

	hDeviceContext = GetDC(hWindow);
}

Win32Context::Win32Context(HWND hwnd, int32 PosX, int32 PosY, int32 Width, int32 Height, bool fullscreen, Win32MessageHandler handler) :
	PosX(PosX),
	PosY(PosY),
	Width(Width),
	Height(Height),
	MessageHandler(handler)
{
	hWindow = hwnd;
	hDeviceContext = GetDC(hWindow);
}


void Win32Context::Show(bool setFocus)
{
	ShowWindow(hWindow, SW_SHOW);

	if (setFocus)
	{
		SetForegroundWindow(hWindow);
		SetFocus(hWindow);
	}

	UpdateWindow(hWindow);
}

void Win32Context::SetPixelFormat(PPIXELFORMATDESCRIPTOR pfd)
{
	::SetPixelFormat(hDeviceContext, 1, pfd);
}

void Win32Context::SetCursorPosition(int x, int y)
{
	if (hWindow)
	{
		SetCursorPos(x, y);
	}
}

void Win32Context::GetCursorPosition(int* x, int* y)
{
	if (hWindow)
	{
		POINT point;

		GetCursorPos(&point);


		*x = point.x;
		*y = point.y;
	}
}

Win32Context::~Win32Context()
{
	ReleaseDC(hWindow, hDeviceContext);
	DestroyWindow(hWindow);
}