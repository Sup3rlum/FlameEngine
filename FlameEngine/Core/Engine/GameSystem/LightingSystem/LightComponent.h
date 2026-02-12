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
	float Depth = 0.0f;
};


struct DirectionalLight : RenderStruct<FVector4, FVector4>
{
	FStaticArray<FViewFrustumInfo, SM_CASCADES> FrustumData;

	FVector3 Direction;
	float Intensity;
	Color32 Color;


	void StageMemory(FRIMemoryMap& GPUMemory, const FMatrix4& View)
	{
		GPUMemory << FVector4(Direction, 0);			// Pad to 16 bytes
		GPUMemory << FVector4(Color.rgb, Intensity);	// Pad to 16 bytes
	}
};


struct PointLight : RenderStruct<FVector4, FVector4, float>
{
	FVector3 Position;
	Color32 Color;
	float Intensity;
	float Radius;

	void StageMemory(FRIMemoryMap& GPUMemory, const FMatrix4& View)
	{
		GPUMemory << FVector4(Position, 1.0f);				// Pad to 16 bytes
		GPUMemory << FVector4(Color.rgb, Intensity);	// Pad to 16 bytes
		GPUMemory << Radius;	

	}
};


struct SpotLight : RenderStruct<FVector4, FVector4, FVector3>
{
	FVector3 Position;
	FVector3 Direction;
	Color32 Color;
	float Intensity;
	float Radius;
	float ApertureSize;
	float ApertureSharpness;

	void StageMemory(FRIMemoryMap& GPUMemory, const FMatrix4& View)
	{
		GPUMemory << FVector4(Position, 1);				// Pad to 16 bytes
		GPUMemory << FVector4(Direction, 0);			// Pad to 16 bytes
		GPUMemory << FVector4(Color.rgb, Intensity);	// Pad to 16 bytes
		GPUMemory << Radius;
		GPUMemory << ApertureSize;
		GPUMemory << ApertureSharpness;
	}
};
