#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"


enum class EPGeometryType
{
	Box,
	Sphere,
	Capsule,
	Plane
};





struct BoxGeometry
{
	FVector3 HalfDim;

	BoxGeometry(FVector3 halfDim) :
		HalfDim(halfDim)
	{
	}

};


struct SphereGeometry
{
	float Radius;

	SphereGeometry(float radius) :
		Radius(radius)
	{
	}

};


struct CapsuleGeometry
{
	float Radius;
	float HalfHeight;

	CapsuleGeometry(float radius, float halfheight) :
		Radius(radius),
		HalfHeight(halfheight)
	{

	}
};

struct PlaneGeometry
{
};
