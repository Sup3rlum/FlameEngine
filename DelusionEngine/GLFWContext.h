#pragma once

#include "BaseContext.h"

EXPORT_CLASS GLFWContext : public BaseContext
{
public:

	void Initialize() override;
	void SwapChain() override;
	void Destroy() override;
	void GetCursorPosition(double* x, double* y) override;
	void SetCursorPosition(double x, double y) override;

	KeyState GetKeyState(Keys key) override;

	GLFWwindow* _windowHandle;
};

