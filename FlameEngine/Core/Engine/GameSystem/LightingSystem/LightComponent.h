#pragma once


#include "../EntityComponent/EntityComponent.h"
#include "Core/Common/CoreCommon.h"
#include "Core/Engine/Renderer/Common/Color.h"


struct PointLightComponent
{
	Color LightColor;
	float Radius;
};



struct SpotLightComponent
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


struct DirectionalLightComponent
{

	FStaticArray<FViewFrustumInfo, 5> FrustumInfo;

	Color LightColor;
	FVector3 Direction;
	float Intensity;
};
