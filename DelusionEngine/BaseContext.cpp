#include "BaseContext.h"


void BaseContext::Initialize()
{

}

void BaseContext::Destroy()
{

}

void BaseContext::GetCursorPosition(double* x, double* y)
{

}
void BaseContext::SetCursorPosition(double x, double y)
{

}
void BaseContext::SwapChain()
{

}

KeyState BaseContext::GetKeyState(Keys key)
{
	return KeyState::RELEASED;
}