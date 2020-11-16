#pragma once


#include "../../dll/nchfx.h"
#include "../../Engine/FrameTime.h"

EXPORT_CLASS UpdatableObject
{
public:
	virtual void Update(FrameTime* _frameTime);


};

