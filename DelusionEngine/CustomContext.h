#pragma once

#include "nchfx.h"
#include "Context.h"
#include <GL/wglew.h>

EXPORT_CLASS CustomContext : public Context
{
public:
	void Initialize(ContextDescription* _cont) override;
	void SwapChain() override;
	void Destroy() override;
	void GetCursorPosition(double* x, double* y) override;
	void SetCursorPosition(double x, double y) override;

	KeyState GetKeyState(Keys key) override;


	void AttachToHandle(HWND _hWnd);
	DELuint32 CustomSetPixelFormat(HDC hdc);

	HWND _handle;
	HDC _hdc;
	HGLRC _hrc;

};