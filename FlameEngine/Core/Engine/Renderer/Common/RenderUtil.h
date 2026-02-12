#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Geometry/VertexComponent.h"



struct FLAME_API FRenderUtil
{
	
#define GLX

#ifdef GL
	inline static FArray<FVertexComponent_PositionTexture> quadVData =
	{
		FVertexComponent_PositionTexture(FVector3(-1,1,0), FVector2(0,1)),
		FVertexComponent_PositionTexture(FVector3(1,1,0), FVector2(1,1)),
		FVertexComponent_PositionTexture(FVector3(1,-1,0), FVector2(1,0)),
		FVertexComponent_PositionTexture(FVector3(-1,-1,0), FVector2(0,0))
		
	};
#else
	
	inline static FArray<FVertex_PositionTexture> quadVertices =
	{
		FVertex_PositionTexture(FVector3(-1,1,0), FVector2(0,0)),
		FVertex_PositionTexture(FVector3(1,1,0), FVector2(1,0)),
		FVertex_PositionTexture(FVector3(1,-1,0), FVector2(1,1)),
		FVertex_PositionTexture(FVector3(-1,-1,0), FVector2(0,1))

	};
#endif

	inline static FArray<FIndexComponent> quadIndices = { 0,2,1,0,3,2 };


	inline static FRIVertexBuffer* VertexBuffer = NULL;
	inline static FRIIndexBuffer* IndexBuffer = NULL;
	inline static FRIInputLayout* VertexDeclaration = NULL;
	inline static FOrthographicMatrix ScreenSpaceMatrix;

	static void CreateResources(FRIDynamicAllocator* Allocator);
	static void DrawScreenQuad(FRICommandList& cmdList);
};

struct FLAME_API FDefaultSamplers
{
	inline static FRISamplerState* Point;
	inline static FRISamplerState* Bilinear;
	inline static FRISamplerState* Trilinear;
	inline static FRISamplerState* Anisotropic4;
	inline static FRISamplerState* Anisotropic8;
};


