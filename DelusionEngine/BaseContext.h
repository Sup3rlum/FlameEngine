#pragma once

#include "nchfx.h"
#include "ContextDescription.h"
#include "Keyboard.h"


EXPORT_CLASS BaseContext
{
public:
	virtual void Initialize();
	virtual void SwapChain();
	virtual void Destroy();
	virtual void SetCursorPosition(double x, double y);
	virtual void GetCursorPosition(double* x, double* y);

	virtual KeyState GetKeyState(Keys Key);


	ContextDescription _contextDescription;
	bool shouldTerminate;
};

