#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/FlameRI/FRI.h"
#include "Core/Engine/FlameRI/ShaderLibrary/ShaderLibrary.h"
#include "../GameSystem/Common/Scene.h"

struct RenderModule
{
	virtual void CreateResources(FRIContext* renderContext) = 0;
	virtual void RecreateResources(FRIContext* context, FRIContext* previousContext = NULL) = 0;

	ShaderLibrary Shaders;
};


struct RStageInterface
{
	FStaticArray<FRIResourceObject*, 16> Buffer;

	template<typename TFRIResourceType>
	TFRIResourceType* GetResource(uint64 index)
	{
		return static_cast<TFRIResourceType*>(Buffer[index]);
	}

};



enum class ERenderStageExecutionMode
{
	None
};

class FRenderStage
{

protected:
	FRenderStage(FStaticAnsiString<32> Name) : Name(Name), Enabled(true) {}
public:

	FStaticAnsiString<32> Name;
	bool Enabled;

	FRIDepthStencilState* DepthStencilState;
	FRIRasterizerState* RasterizerState;
	FRIBlendState* BlendState;

	virtual void CreateResources(ShaderLibrary& library, FRIContext* context) = 0;
	virtual void RecreateResources(ShaderLibrary& library, FRIContext* context, FRIContext* previousContext = NULL) = 0;


	virtual void Prepare(FRICommandList& cmdList, RStageInterface& rstageinterface) = 0;
	virtual void SubmitPass(FRICommandList& cmdList, Scene* scene) = 0;
	virtual void Finish(FRICommandList& cmdList, RStageInterface& rstageinterface) = 0;

	virtual ~FRenderStage()
	{

	}
};

