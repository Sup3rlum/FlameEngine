#pragma once

#include "Core/Common/CoreCommon.h" 
#include "Core/Engine/FlameRI/FRICommandList.h"

EXPORT(class, RenderableObjectBase)
{
public:
	FRICommandList renderCommandList;
};



EXPORT(class, DynamicRenderableObject) : RenderableObjectBase
{
};

