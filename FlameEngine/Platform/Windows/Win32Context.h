#pragma once


#include "Core/Common/CoreCommon.h"


typedef FDelegate<LRESULT(HWND, UINT, WPARAM, LPARAM)> FWin32MessageProcDelegate;

struct Win32MessageHandler
{
	FWin32MessageProcDelegate Handler;

	Win32MessageHandler(FWin32MessageProcDelegate del) : Handler(del)
	{
	}

};

EXPORT(struct, DefaultWin32MessageHandler) : public Win32MessageHandler
{
	DefaultWin32MessageHandler();
};


EXPORT(struct, Win32Context)
{
	HWND hWindow;
	HDC hDeviceContext;

	FString Name;
	int32 PosX;
	int32 PosY;
	int32 Width;
	int32 Height;

	Win32MessageHandler MessageHandler;

	Win32Context(FString Name, int32 PosX, int32 PosY, int32 Width, int32 Height, Win32MessageHandler handler = DefaultWin32MessageHandler());
	Win32Context(HWND hwnd, int32 PosX, int32 PosY, int32 Width, int32 Height, Win32MessageHandler handler = DefaultWin32MessageHandler());
	void Show(bool setFocus = true);

	void SetPixelFormat(PPIXELFORMATDESCRIPTOR pfd);
	void SetCursorPosition(int x, int y);
	void GetCursorPosition(int* x, int* y);
	~Win32Context();

private:

	static LRESULT CALLBACK Win32MessageProcSignature(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

};