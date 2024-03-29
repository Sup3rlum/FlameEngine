#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Geometry/VertexComponent.h"



struct FRenderUtil
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
	
	inline static FArray<FVertex_PositionTexture> quadVData =
	{
		FVertex_PositionTexture(FVector3(-1,1,0), FVector2(0,0)),
		FVertex_PositionTexture(FVector3(1,1,0), FVector2(1,0)),
		FVertex_PositionTexture(FVector3(1,-1,0), FVector2(1,1)),
		FVertex_PositionTexture(FVector3(-1,-1,0), FVector2(0,1))

	};
#endif

	inline static FArray<FIndexComponent> quadElementData = { 0,2,1,0,3,2 };


	inline static FRIVertexBuffer* VertexBuffer = NULL;
	inline static FRIIndexBuffer* IndexBuffer = NULL;
	inline static FRIVertexDeclaration* VertexDeclaration = NULL;

	inline static FArray<FRIInputAttribute> VertexDeclComp =
	{
		FRIInputAttribute("POSITION", 3, EFRIAttributeType::Float, EFRIBool::False, 20, 0),
		FRIInputAttribute("TEXCOORD", 2, EFRIAttributeType::Float, EFRIBool::False, 20, 12)
	};



	inline static FOrthographicMatrix ScreenSpaceMatrix;

	static void DrawScreenQuad(FRICommandList& cmdList)
	{
		cmdList.SetGeometrySource(VertexBuffer);
		cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, 6, EFRIIndexType::UInt32, IndexBuffer);
	}
};


