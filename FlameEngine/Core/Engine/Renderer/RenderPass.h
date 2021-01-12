#pragma once

#include "../../dll/nchfx.h"


EXPORT(enum class, DeferredSceneRenderMode)
{
	DEPTH_PREPASS,
	GEOMETRY_PASS,
	COMBINE_PASS
};

