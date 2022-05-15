#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "../GameSystem/Common/Scene.h"

EXPORT(struct, RenderModule)
{
	virtual void CreateResources(FRIContext* renderContext) = 0;
	virtual void RecreateResources(FRIContext* context, FRIContext* previousContext = NULL) = 0;

	void AttachToScene(Scene* scene);

	ShaderLibrary Shaders;
	Scene* scene;

	FArray<RenderModule*> RenderModules;

};


struct RStageInterface
{
	FStaticArray<FRIResourceObject*, 24> Buffer;

	template<typename TFRIResourceType>
	TFRIResourceType* GetResource(uint64 index)
	{
		return static_cast<TFRIResourceType*>(Buffer[index]);
	}

};