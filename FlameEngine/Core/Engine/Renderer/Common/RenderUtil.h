#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Geometry/VertexComponent.h"



struct FScreenQuadUtil
{
	inline static FArray<FVertexComponent_PositionTexture> quadVData =
	{
		FVertexComponent_PositionTexture(FVector3(-1,1,0), FVector2(0,1)),
		FVertexComponent_PositionTexture(FVector3(1,1,0), FVector2(1,1)),
		FVertexComponent_PositionTexture(FVector3(1,-1,0), FVector2(1,0)),
		FVertexComponent_PositionTexture(FVector3(-1,-1,0), FVector2(0,0))

	};

	inline static FArray<FIndexComponent> quadElementData = { 0,2,1,0,3,2 };


	inline static FResourceVertexBuffer* VertexBuffer = NULL;
	inline static FResourceIndexBuffer* IndexBuffer = NULL;


	inline static FArray<FResourceVertexDeclarationComponent> VertexDecl =
	{
		FResourceVertexDeclarationComponent(0, 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 20, 0),
		FResourceVertexDeclarationComponent(1, 2, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 20, 12)
	};

	inline static FOrthographicMatrix ScreenSpaceMatrix;

};


