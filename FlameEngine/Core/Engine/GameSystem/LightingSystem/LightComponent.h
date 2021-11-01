#pragma once


#include "../EntityComponent/EntityComponent.h"
#include "Core/Common/CoreCommon.h"
#include "Core/Engine/Renderer/Common/Color.h"

#define SM_CASCADES 5

struct PointLight
{
	FVector4 Position;
	Color LightColor;
	float Intensity;
	float Radius;
};



struct SpotLight
{
	Color LightColor;
	float Radius;
};

struct FViewFrustumInfo
{
	FMatrix4 View;
	FMatrix4 Projection;
	FVector4 Depth;
};


struct DirectionalLight
{

	FStaticArray<FViewFrustumInfo, SM_CASCADES> FrustumInfo;

	Color LightColor;
	FVector3 Direction;
	float Intensity;
};
