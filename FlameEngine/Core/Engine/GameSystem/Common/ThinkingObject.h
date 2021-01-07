#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/Common/FTime.h"



EXPORT(class, ThinkingObject)
{
protected:
	FTimeStamp LastThink;
public:
	virtual void Think();


};

