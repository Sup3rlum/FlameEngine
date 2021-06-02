#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"


struct RenderModule
{
	virtual void CreateResources(FRIContext* renderContext) = 0;

	ShaderLibrary Shaders;
};