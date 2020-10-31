#include "Context.h"


void Context::Initialize(ContextDescription* _contextDesc)
{

	_contextDescription = _contextDesc;
	FrameTime::Start();
	shouldTerminate = false;
}

void Context::SwapChain()
{
	FrameTime::FrameDeltaTime = FrameTime::Now() - FrameTime::TotalTime;
	FrameTime::TotalTime = FrameTime::Now();
	FrameTime::EllapsedFrames++;
}

void Context::Destroy()
{

}

void Context::PollClose()
{
	shouldTerminate = true;
}

void Context::GetCursorPosition(double* x, double* y)
{

}
void Context::SetCursorPosition(double x, double y)
{

}

KeyState Context::GetKeyState(Keys key)
{
	return KeyState::RELEASED;
}