#pragma once


#include "Core/Common/CoreCommon.h"

EXPORT(class, FDisplacement) : public FVector3 
{

};


EXPORT(class, FVelocity) : public FVector3 
{

};

EXPORT(class, FAcceleration) : public FVector3 
{

};


EXPORT(class, FJerk) : public FVector3 
{

};


EXPORT(class, FTorque) : public FVector3 
{

};


EXPORT(class, FForce) : public FAcceleration