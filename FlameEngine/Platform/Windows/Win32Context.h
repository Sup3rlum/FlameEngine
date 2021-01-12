#pragma once


#include "Core/Common/CoreCommon.h"

#include <windows.h>


typedef FDelegate<LRESULT(HWND, UINT, WPARAM, LPARAM)> FWin32MessageProcDelegate;

struct Win32MessageHandler
{
	FWin32MessageProcDelegate Handler;

	Win32MessageHandler(FWin32MessageProcDelegate del) : Handler(del)
	{

	}

};

struct DefaultWin32MessageHandler : public Win32MessageHandler
{

public:
	DefaultWin32MessageHandler() :
		Win32MessageHandler([](HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
			{
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			})
	{
	}

};


struct Win32Context
{
	HWND hWindow;
	HDC hDeviceContext;

	FString Name;
	int32 PosX;
	int32 PosY;
	int32 Width;
	int32 Height;

	Win32MessageHandler MessageHandler;

	Win32Context(FString Name, int32 PosX, int32 PosY, int32 Width, int32 Height, Win32MessageHandler handler = DefaultWin32MessageHandler()) :
		Name(Name),
		PosX(PosX),
		PosY(PosY),
		Width(Width),
		Height(Height),
		MessageHandler(handler)
	{

		WNDCLASSEX wc;

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = Win32MessageProcSignature;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = NULL;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = Name.ToWCString();
		wc.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&wc);



		hWindow = CreateWindowEx(
			WS_EX_APPWINDOW,
			Name.ToWCString(),
			Name.ToWCString(),
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

	void Show(bool setFocus = true)
	{
		ShowWindow(hWindow, SW_SHOW);

		if (setFocus)
		{
			SetForegroundWindow(hWindow);
			SetFocus(hWindow);
		}
	}

	void SetPixelFormat(PPIXELFORMATDESCRIPTOR pfd)
	{
		::SetPixelFormat(hDeviceContext, 1, pfd);
	}

	~Win32Context()
	{
		ReleaseDC(hWindow, hDeviceContext);
		DestroyWindow(hWindow);
	}


private:

	static LRESULT CALLBACK Win32MessageProcSignature(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

};