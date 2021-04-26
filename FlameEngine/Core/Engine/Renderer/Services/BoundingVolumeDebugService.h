#pragma once


#include "../../GameSystem/CameraSystem/CameraComponent.h"
#include "../Common/Geometry/VertexComponent.h"

#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Framework/IO/FileStream.h"

EXPORT(class, BoundingVolumeDebugService) 
{
public:
	BoundingVolumeDebugService(FRICommandList& cmdList);
	void Render(FRICommandList& cmdList, const CameraComponent& cam, FVector3* Corners);

	FResourceShaderPipeline* pipeline;
	FResourceVertexBuffer* vBuffer;

	uint32 ViewLoc;
	uint32 ProjLoc;

	FResourceVertexDeclaration vDecl;

};
