#pragma once

#include "../FlameRI/FRI.h"

enum class GRenderMode
{
	Shadow,
	Material,
	Transluscent
};

struct RenderList
{
	virtual void AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode) = 0;
};

struct RenderObject
{
	RenderList* list = NULL;
};