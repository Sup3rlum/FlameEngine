#include "FrameTime.h"

FrameTime::FrameTime()
{
	DeltaTime = glfwGetTime();
	TotalTime = glfwGetTime();
}
