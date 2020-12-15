#pragma once


#include "../../dll/nchfx.h"
#include "../../Engine/FrameTime.h"

EXPORT(class,  UpdatableObject)
{
public:
	virtual void Update(FrameTime* _frameTime);


};

