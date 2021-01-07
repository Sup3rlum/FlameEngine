#pragma once

#include "Core/Common/CoreCommon.h"
//#include "FVector3.h"
//#include "FVector4.h"
//#include "ITimeDimensional.h"
//#include "IMassDimensional.h"


EXPORT(class, FDisplacement) : public FVector3 : public ITimeDimensional<0>
{

};


EXPORT(class, FVelocity) : public FVector3 : public ITimeDimensional<1>
{

};

EXPORT(class, FAcceleration) : public FVector3 : public ITimeDimensional<2>
{

};


EXPORT(class, FJerk) : public FVector3 : public ITimeDimensional<3>
{

};


EXPORT(class, FTorque) : public FVector3 : public ITimeDimensional<2>
{

};


EXPORT(class, FForce) : public FAcceleration : public IMassDimensional<1> {};