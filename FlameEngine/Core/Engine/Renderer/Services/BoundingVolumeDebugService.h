#pragma once


#include "../../GameSystem/CameraSystem/CameraComponent.h"
#include "../Common/Geometry/VertexComponent.h"

#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Framework/IO/FileStream.h"
#include "../RenderModule.h"

class BoundingVolumeDebugService : RenderModule
{
public:
	BoundingVolumeDebugService();
	void Render(FRICommandList& cmdList, FVector3* Corners);

	void CreateResources(FRIContext* cmdList);

	FRIShaderPipeline* pipeline;
	FRIVertexBuffer* vBuffer;

	uint32 ViewLoc;
	uint32 ProjLoc;

	FRIVertexDeclaration* VertexDecl;

};
