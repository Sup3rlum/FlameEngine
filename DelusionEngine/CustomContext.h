#pragma once

#include "nchfx.h"
#include "BaseContext.h"

EXPORT_CLASS CustomContext : public BaseContext
{
public:
	void Initialize() override;
	void SwapChain() override;
	void Destroy() override;
	void GetCursorPosition(double* x, double* y) override;
	void SetCursorPosition(double x, double y) override;

	KeyState GetKeyState(Keys key) override;
};

