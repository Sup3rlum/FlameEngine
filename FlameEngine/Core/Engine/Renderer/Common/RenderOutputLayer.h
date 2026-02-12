#pragma once


#include "Core/Engine/FlameRI/FRI.h"

struct RenderOutputLayer
{
	FRIRenderTargetView* FrameBuffer;
	FString8 Name;
};