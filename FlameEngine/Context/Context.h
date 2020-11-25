#pragma once

#include "../dll/nchfx.h"
#include "ContextDescription.h"
#include "../Engine/FrameTime.h"
#include "../IO/Input/Keyboard.h"
#include "../IO/Input/Mouse.h"


class Context
{
public:

	virtual void Initialize(ContextDescription* _contextDescription);
	virtual void SwapChain();
	virtual void Destroy();
	virtual void PollClose();

	virtual void GetCursorPosition(double* x, double* y);
	virtual void SetCursorPosition(double x, double y);


	virtual void SetCursorVisible(bool visible);


	virtual KeyState GetKeyState(Keys key);
	virtual MouseButtonState GetMouseButtonState(MouseButton mbutton);

	bool shouldTerminate;
	ContextDescription* _contextDescription;
};

