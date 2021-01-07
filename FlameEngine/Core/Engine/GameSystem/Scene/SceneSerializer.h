#pragma once

#include "Core/Common/CoreCommon.h"


EXPORT(struct, FSerializerArgs)
{

};

EXPORT(struct, FSceneSerializerBase)
{
	virtual void Serialize() = 0;
};

