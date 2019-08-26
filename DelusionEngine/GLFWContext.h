#pragma once

#include "BaseContext.h"

EXPORT_CLASS GLFWContext : public BaseContext
{
	void Initialize() override;
	void SwapChain() override;
	void Destroy() override;
	void GetCursorPosition(double* x, double* y) override;
	void SetCursorPosition(double x, double y) override;

	GLFWwindow* _windowHandle;
};

