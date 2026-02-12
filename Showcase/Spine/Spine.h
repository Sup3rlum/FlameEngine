#pragma once

#include <FlameEngine/Core/Engine/GameSystem/RenderObject.h>
#include "FlameEngine/Core/Engine/ContentSystem/ImportScripts/Common.h"
#include <FlameEngine/Core/Engine/ContentSystem/Client/LocalAssetManager.h>
#include <FlameEngine/Core/Engine/Renderer/Common/Geometry/VertexComponent.h>





class Spine : public RenderList
{
public:
	Material material;
	FRIContext* FriContext;
	FRITexture3D* SDF;
	FRIShaderResourceView* SDFsrv;
	FRIPipelineStateObject* SpineRender;

	FRIPipelineStateObject* CreateGeometryPipelineState(const ShaderLibraryModule& Shaders);

	Spine(FRIContext* FriContext);
	void AddToCmdList(FRICommandList& cmdList, GRenderMode renderMode) override;
};