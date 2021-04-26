#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Common/Viewport.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"
#include "Common/RenderUtil.h"
#include "Core/Framework/IO/FileStream.h"

#include "../GameSystem/MeshComponent.h"
#include "../ContentSystem/Client/AssetImportScripts/Mesh.h"

class AtmosphereRenderer
{
public:

	void CreateResources(FRIContext* renderContext);

	void RenderSkySphere(FRICommandList& cmdList, CameraComponent& cameraRef, TransformComponent& cameraTransform);


	uint32 WorldLoc;
	uint32 ViewLoc;
	uint32 ProjLoc;

	uint32 CameraPosLoc;
	uint32 LightDirectionLoc;


	FResourceShaderPipeline* pipeline;
	
	
	MeshComponent renderSphere;

};