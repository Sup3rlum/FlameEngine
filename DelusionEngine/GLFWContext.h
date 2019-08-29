#pragma once

#include "nchfx.h"
#include "ContextDescription.h"
#include "Keyboard.h"
#include "Context.h"

EXPORT_CLASS GLFWContext : public Context
{
public:

	void Initialize(ContextDescription* _contextDescription) override;
	void SwapChain() override;
	void Destroy() override;
	void PollClose() override;

	void GetCursorPosition(double* x, double* y) override;
	void SetCursorPosition(double x, double y) override;

	KeyState GetKeyState(Keys key) override;

	GLFWwindow* _windowHandle;
};

