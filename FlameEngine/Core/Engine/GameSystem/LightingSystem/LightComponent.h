#pragma once


#include "../EntityComponent/EntityComponent.h"
#include "Core/Common/CoreCommon.h"
#include "Core/Engine/Renderer/Common/Color.h"

#define SM_CASCADES 5
 

struct FViewFrustumInfo
{
	//FMatrix4 ToLightTransform;
	FMatrix4 View;
	FMatrix4 Projection;
	float Depth;
};




struct DirectionalLight
{
	FStaticArray<FViewFrustumInfo, SM_CASCADES> FrustumInfo;

	FVector3 Direction;
	Color Color;
	float Intensity;


	void StageMemory(FRIMemoryMap& memory)
	{
		memory << FVector4(Direction, 0);			// Pad to 16 bytes
		memory << FVector4(Color.rgb, Intensity);	// Pad to 16 bytes
	}

	ENTITY_STAGE(FVector4, FVector4)
};


struct PointLight
{
	FVector3 Position;
	Color Color;
	float Intensity;
	float Radius;

	void StageMemory(FRIMemoryMap& memory)
	{
		memory << FVector4(Position, 1.0f);				// Pad to 16 bytes
		memory << FVector4(Color.rgb, Intensity);	// Pad to 16 bytes
		memory << Radius;	

	}

	ENTITY_STAGE(FVector4, FVector4, float)
};


struct SpotLight
{
	FVector3 Position;
	FVector3 Direction;
	Color Color;
	float Intensity;
	float Radius;
	float ApertureSize;
	float ApertureSharpness;

	void StageMemory(FRIMemoryMap& memory)
	{
		memory << FVector4(Position, 1);				// Pad to 16 bytes
		memory << FVector4(Direction, 0);			// Pad to 16 bytes
		memory << FVector4(Color.rgb, Intensity);	// Pad to 16 bytes
		memory << Radius;
		memory << ApertureSize;
		memory << ApertureSharpness;

	}

	ENTITY_STAGE(FVector4, FVector4, float, float, float)
};
