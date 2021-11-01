#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"



#include "Services/BoundingVolumeDebugService.h"

#include "AtmosphereRenderer.h"

#include "Core/Engine/ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"

#include "Deferred/RenderStages.h"






EXPORT(class, DeferredRenderer) : public RenderModule
{
public:

	bool isDebug = false;

	void CreateResources(FRIContext* renderContext);
	void RecreateResources(FRIContext* renderContext, FRIContext* prevContext = 0);


	void BeginRender(FRICommandList & cmdList);
	void Render(FRICommandList& cmdList);
	void EndRender(FRICommandList & cmdList);


	void AttachToScene(Scene* scene);


	FArray<FRenderStage*> RenderStages;
	AtmosphereRenderer atmosphereRenderer;

	Scene* scene = NULL;

	BoundingVolumeDebugService* bvds;
};