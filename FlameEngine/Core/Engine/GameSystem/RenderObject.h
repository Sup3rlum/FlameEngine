#pragma once

#include "../FlameRI/FRI.h"

enum class GRenderMode
{
	Shadow,
	Material,
	Transluscent,
	PreLighting,
	PostLighting
};

struct RenderList
{
	virtual void AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode) = 0;
};

struct RenderObject
{
	RenderList* RenderList = NULL;
};