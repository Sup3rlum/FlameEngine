#pragma once

#include "nchfx.h"
#include "ContextDescription.h"
#include "FrameTime.h"
#include "Keyboard.h"


class Context
{
public:

	virtual void Initialize(ContextDescription* _contextDescription);
	virtual void SwapChain();
	virtual void Destroy();
	virtual void PollClose();

	virtual void GetCursorPosition(double* x, double* y);
	virtual void SetCursorPosition(double x, double y);

	virtual KeyState GetKeyState(Keys key);

	bool shouldTerminate;
	ContextDescription* _contextDescription;
};

