#pragma once

#include "nchfx.h"
#include "ContextDescription.h"

EXPORT_CLASS BaseContext
{
public:
	virtual void Initialize();
	virtual void SwapChain();
	virtual void Destroy();
	virtual void SetCursorPosition(double x, double y);
	virtual void GetCursorPosition(double* x, double* y);


	ContextDescription _contextDescription;
	bool shouldTerminate;
};

