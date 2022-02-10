#pragma once


#include "../GameSystem/CameraSystem/CameraComponent.h"
#include "Common/Geometry/VertexComponent.h"

#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Framework/IO/FileStream.h"
#include "RenderModule.h"

class DebugRenderer : public RenderModule
{
public:
	DebugRenderer();

	void Render(FRICommandList& cmdList, FVector3* Corners);
	void CreateResources(FRIContext* cmdList);
	void RecreateResources(FRIContext* cmdList, FRIContext* previous);

	FRIShaderPipeline* pipeline;
	FRIVertexBuffer* vBuffer;


	FRIVertexDeclaration* VertexDecl;

};
