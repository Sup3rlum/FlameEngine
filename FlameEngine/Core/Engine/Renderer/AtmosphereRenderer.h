#pragma once


#include "RenderModule.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"
#include "Common/RenderUtil.h"
#include "Core/Framework/IO/FileStream.h"

#include "../GameSystem/MeshComponent.h"
#include "../ContentSystem/Client/AssetImportScripts/Mesh.h"
#include "../ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"

class AtmosphereRenderer : RenderModule
{
public:

	void CreateResources(FRIContext* renderContext);
	void RenderSkySphere(FRICommandList& cmdList, CameraComponent& cameraRef, FTransform cameraTransform);


	FRIShaderPipeline* pipeline;
	FRIUniformBuffer* ConstantBuffer;
	

	MeshComponent renderSphere;

};