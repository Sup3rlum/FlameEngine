#pragma once

#include "FRIResource.h"
#include "FRIContext.h"
#include "Core/Runtime/Common/MemoryStack.h"

/*
* 
*	I have defined some hacky-ish macros to help code commands faster and help sake readability.
* 
*	FRegiterCommand creates a struct with set cmdName prefixed with FRICommand
*	e.g. FRegisterCommand(CallMom) -> struct FRICommandCallMom : FRICommand<FRICommandCallMom>
* 
*	FRegisterCommandMultiTemplate does the same job, but allows for template specialization, slotting the generic
*	parameterizations in their proper places upon declaration
*	e.g. FRegisterCommand(CallDad, TGenType1, TGenType2, TGenType3) ->
*	
*	template<typename TGenType1, typename TGenType2, typename TGenType3>
*	struct FRICommandCallDad : FRICommand<FRICommandCallDad<TGenType1, TGenType2,TGenType3>>
* 
*	FRICmdInit initializes a __forceinline constructor with the prefixed name matching the struct definition name
* 
*/

#define FTEMPLATETYPENAME(genType) typename genType


#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N


#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x) _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x) _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x) _fe_3(_call, __VA_ARGS__)

#define CALL_MACRO_X_FOR_EACH(x, ...) \
    _GET_NTH_ARG("ignored", ##__VA_ARGS__, \
    _fe_4, _fe_3, _fe_2, _fe_1, _fe_0)(x, ##__VA_ARGS__)


#define FRegisterFRICommand(cmdName) struct FRICommand##cmdName : FRICommand<FRICommand##cmdName>

#define FRegisterFRICommandMultiTemplate(cmdName, ...) template<CALL_MACRO_X_FOR_EACH(FTEMPLATETYPENAME, __VA_ARGS__)> \
														struct FRICommand##cmdName : FRICommand<FRICommand##cmdName<__VA_ARGS__>>


#define FRICmdInit(name) FORCEINLINE FRICommand##name


struct FRICommandListBase;




struct FRICommandBase
{
	FRICommandBase* Next = NULL;
	uint32 CommandIdx = 0;

	virtual void ExecuteCmd(FRICommandListBase& cmdList) = 0;
};


struct FRICommandListBase
{
	FRICommandListBase() :
		First(0),
		CommandLink(0),
		NumCommands(0),
		FriDynamic(0)
	{
		CommandLink = &First;
	}

	FORCEINLINE void* AllocCommand(int32 AllocSize, int32 Alignment)
	{
		FRICommandBase* Result = (FRICommandBase*)MemoryStack.Alloc(AllocSize, Alignment);
		++NumCommands;
		*CommandLink = Result;
		CommandLink = &Result->Next;
		return Result;
	}
	template <typename TCmd>
	FORCEINLINE void* AllocCommand()
	{
		return AllocCommand(sizeof(TCmd), alignof(TCmd));
	}


	FRIDynamicAllocator* GetDynamic() const
	{
		return FriDynamic;
	}
	void Flush();

	FMemoryStack MemoryStack;
	FRICommandBase* First;
	FRICommandBase** CommandLink;
	uint32 NumCommands;


	FRIDynamicAllocator* FriDynamic;
};





template<typename FCmdParam>
struct FRICommand : FRICommandBase
{
	FORCEINLINE void ExecuteCmd(FRICommandListBase& cmdList)
	{
		FCmdParam* cmd = static_cast<FCmdParam*>(this);

		cmd->Execute(cmdList);
		cmd->~FCmdParam();
	}

};


/*
* 
*	VIEWPORT
* 
* 
*/

FRegisterFRICommand(SetViewport)
{
	uint32 x;
	uint32 y;
	uint32 width;
	uint32 height;

	FRICmdInit(SetViewport)(uint32 x, uint32 y, uint32 width, uint32 height) :
		x(x),
		y(y),
		width(width),
		height(height)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};

/*
* 
*  FRAMEBUFFER
* 
*/


FRegisterFRICommand(BindFrameBuffer)
{
	FRIFrameBuffer* frameBuffer;

	FRICmdInit(BindFrameBuffer)(FRIFrameBuffer* frameBuffer) :
		frameBuffer(frameBuffer)
	{

	}

	void Execute(FRICommandListBase& cmdList);

};

FRegisterFRICommand(UnbindFrameBuffer)
{
	FRICmdInit(UnbindFrameBuffer)()
	{

	}

	void Execute(FRICommandListBase & cmdList);

};


FRegisterFRICommand(ClearBufferColor)
{
	FRIFrameBuffer* frameBuffer;
	Color color;

	FRICmdInit(ClearBufferColor)(FRIFrameBuffer* frameBuffer, Color color) :
		frameBuffer(frameBuffer),
		color(color)
	{

	}

	void Execute(FRICommandListBase& cmdList);

};

FRegisterFRICommand(ClearBuffer)
{
	FRIFrameBuffer* frameBuffer;
	Color clearcolor;

	FRICmdInit(ClearBuffer)(FRIFrameBuffer* frameBuffer, Color clearcolor) :
		frameBuffer(frameBuffer),
		clearcolor(clearcolor)
	{

	}

	void Execute(FRICommandListBase& cmdList);

};


FRegisterFRICommandMultiTemplate(CopyBufferData, GenFrameBufferPtr)
{
	GenFrameBufferPtr frameBufferSource;
	GenFrameBufferPtr frameBufferDest;
	uint32 clearbit;

	FRICmdInit(CopyBufferData)(GenFrameBufferPtr frameBufferSource, GenFrameBufferPtr frameBufferDest, uint32 clearbit) :
		frameBufferSource(frameBufferSource),
		frameBufferDest(frameBufferDest),
		clearbit(clearbit)
	{

	}

	void Execute(FRICommandListBase& cmdList);

};


FRegisterFRICommand(SetDepthCompareFunc)
{
	uint32 depthfunc;
	FRICmdInit(SetDepthCompareFunc)(uint32 depthfunc) :
		depthfunc(depthfunc)
	{

	}
	void Execute(FRICommandListBase& cmdList);
};



FRegisterFRICommand(SetAlphaBlendFunc)
{
	uint32 blendfunc;
	FRICmdInit(SetAlphaBlendFunc)(uint32 blendfunc) :
		blendfunc(blendfunc)
	{

	}
	void Execute(FRICommandListBase& cmdList);
};



FRegisterFRICommand(SetPrimitiveCullMode)
{
	uint32 cullmode;
	FRICmdInit(SetPrimitiveCullMode)(uint32 cullmode) :
		cullmode(cullmode)
	{

	}
	void Execute(FRICommandListBase& cmdList);
};




FRegisterFRICommand(BeginFrame)
{
	FRICmdInit(BeginFrame)()
	{

	}
	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommand(EndFrame)
{
	FRICmdInit(EndFrame)()
	{

	}
	void Execute(FRICommandListBase& cmdList);
};

FRegisterFRICommand(BeginScene)
{
	FRICmdInit(BeginScene)()
	{

	}
	void Execute(FRICommandListBase& cmdList);
};

FRegisterFRICommand(EndScene)
{
	FRICmdInit(EndScene)()
	{

	}
	void Execute(FRICommandListBase& cmdList);
};


/*
* 
*  BUFFER RENDERING
* 
*/


FRegisterFRICommand(SetGeometrySource)
{
	FRIVertexBuffer* vertexBuffer;

	FRICmdInit(SetGeometrySource)(FRIVertexBuffer * vertexBuffer) :
		vertexBuffer(vertexBuffer)
	{

	}
	void Execute(FRICommandListBase& cmdList);

};



FRegisterFRICommand(DrawPrimitives)
{
	EFRIPrimitiveType primitiveType;
	uint32 vertexCount;

	FRICmdInit(DrawPrimitives)(EFRIPrimitiveType primitiveType, uint32 vertexCount) :
		primitiveType(primitiveType),
		vertexCount(vertexCount)
	{
		
	}

	void Execute(FRICommandListBase& cmdList);

};

FRegisterFRICommand(DrawPrimitivesIndexed)
{

	EFRIPrimitiveType primitiveType;
	uint32 indexCount;
	EFRIIndexType indexType;
	FRIIndexBuffer* indexBuffer;

	FRICmdInit(DrawPrimitivesIndexed)(EFRIPrimitiveType primitiveType, uint32 indexCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer) :
		primitiveType(primitiveType),
		indexCount(indexCount),
		indexType(indexType),
		indexBuffer(indexBuffer)
	{

	}

	void Execute(FRICommandListBase& cmdList);

};



FRegisterFRICommand(DrawInstances)
{
	EFRIPrimitiveType primitiveType;
	uint32 vertexCount;
	uint32 instanceCount;

	FRICmdInit(DrawInstances)(EFRIPrimitiveType primitiveType, uint32 vertexCount, uint32 instanceCount) :
		primitiveType(primitiveType),
		vertexCount(vertexCount),
		instanceCount(instanceCount)
	{

	}

	void Execute(FRICommandListBase & cmdList);

};

FRegisterFRICommand(DrawInstancesIndexed)
{

	EFRIPrimitiveType primitiveType;
	uint32 indexCount;
	uint32 instanceCount;
	EFRIIndexType indexType;
	FRIIndexBuffer* indexBuffer;

	FRICmdInit(DrawInstancesIndexed)(EFRIPrimitiveType primitiveType, uint32 indexCount, uint32 instanceCount, EFRIIndexType indexType, FRIIndexBuffer * indexBuffer) :
		primitiveType(primitiveType),
		indexCount(indexCount),
		instanceCount(instanceCount),
		indexType(indexType),
		indexBuffer(indexBuffer)
	{

	}

	void Execute(FRICommandListBase & cmdList);

};

FRegisterFRICommand(SetFrameBufferTextureLayer)
{
	FRIFrameBuffer* frameBuffer;
	uint32 layer;

	FRICmdInit(SetFrameBufferTextureLayer)(FRIFrameBuffer * frameBuffer, uint32 layer) :
		frameBuffer(frameBuffer),
		layer(layer)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


/*
* 
* SHADERS
* 
*/



FRegisterFRICommand(SetShaderPipeline)
{
	FRIShaderPipeline* shader;

	FRICmdInit(SetShaderPipeline)(FRIShaderPipeline* shader) :
		shader(shader)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommand(SetShaderUniformBuffer)
{
	uint32 slot;
	FRIUniformBuffer* uniformBuffer;

	FRICmdInit(SetShaderUniformBuffer)(uint32 slot, FRIUniformBuffer* uniformBuffer) :
		slot(slot),
		uniformBuffer(uniformBuffer)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommand(SetShaderSampler)
{
	FRISampler sampler;

	FRICmdInit(SetShaderSampler)(FRISampler sampler) :
		sampler(sampler)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};

/*
* 
* TEXTURES
* 
*/


FRegisterFRICommand(SetTextureStorage2D)
{
	uint32 innerColorFormat;
	uint32 coverColorFormat;
	uint32 dataColorFormat;

	FRICmdInit(SetTextureStorage2D)(uint32 innerColorFormat, uint32 coverColorFormat, uint32 dataColorFormat) :
		innerColorFormat(innerColorFormat),
		coverColorFormat(coverColorFormat),
		dataColorFormat(dataColorFormat)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommand(SetTextureStorage3D)
{
	uint32 innerColorFormat;
	uint32 coverColorFormat;
	uint32 dataColorFormat;

	FRICmdInit(SetTextureStorage3D)(uint32 innerColorFormat, uint32 coverColorFormat, uint32 dataColorFormat):
		innerColorFormat(innerColorFormat),
		coverColorFormat(coverColorFormat),
		dataColorFormat(dataColorFormat)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommandMultiTemplate(SetTextureParameterBuffer, GenResourceTexture)
{
	GenResourceTexture* texture;
	FRITextureParameterBuffer parameterBuffer;

	FRICmdInit(SetTextureParameterBuffer)(GenResourceTexture* texture, FRITextureParameterBuffer parameterBuffer) :
		texture(texture),
		parameterBuffer(parameterBuffer)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommandMultiTemplate(FlushMipMaps, GenResourceTexture)
{
	GenResourceTexture* texture;

	FRICmdInit(FlushMipMaps)(GenResourceTexture * texture) :
		texture(texture)
	{

	}

	void Execute(FRICommandListBase & cmdList);
};


/*
* 
* 
* COMPUTE
* 
* 
*/





/*
* 
* 
* MISC
* 
* 
*/


FRegisterFRICommandMultiTemplate(BeginOcclusionQuery, GenOcclusionQueryPtr)
{
	GenOcclusionQueryPtr occlusionQuery;

	FRICmdInit(BeginOcclusionQuery)(GenOcclusionQueryPtr occlusionQuery) :
		occlusionQuery(occlusionQuery)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommandMultiTemplate(EndOcclusionQuery, GenOcclusionQueryPtr)
{
	GenOcclusionQueryPtr occlusionQuery;

	FRICmdInit(EndOcclusionQuery)(GenOcclusionQueryPtr occlusionQuery) :
		occlusionQuery(occlusionQuery)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommand(SetRasterizerState)
{
	FRIRasterizerState* rasterizer;
	
	FRICmdInit(SetRasterizerState)(FRIRasterizerState * rasterizer) :
		rasterizer(rasterizer)
	{

	}
	void Execute(FRICommandListBase& cmdList);
};


FRegisterFRICommand(SetBlendState)
{
	FRIBlendState* blend;

	FRICmdInit(SetBlendState)(FRIBlendState * blend) :
		blend(blend)
	{

	}
	void Execute(FRICommandListBase & cmdList);
};

FRegisterFRICommand(SetDepthStencilState)
{
	FRIDepthStencilState* depth;

	FRICmdInit(SetDepthStencilState)(FRIDepthStencilState* depth) :
		depth(depth)
	{

	}
	void Execute(FRICommandListBase & cmdList);
};

FRegisterFRICommand(UniformBufferSubdata)
{
	FRIUniformBuffer* buffer;
	FRIUpdateDescriptor data;

	FRICmdInit(UniformBufferSubdata)(FRIUniformBuffer* buffer, FRIUpdateDescriptor desc) :
		buffer(buffer),
		data(desc)
	{

	}
	void Execute(FRICommandListBase & cmdList);
};

FRegisterFRICommand(StageResources)
{
	FRIUniformBuffer* buffer;
	FRIMemoryStageDelegate delegate;

	FRIUpdateDescriptor update;

	FRICmdInit(StageResources)(FRIUniformBuffer* buffer, FRIMemoryStageDelegate delegate) :
		buffer(buffer),
		delegate(delegate),
		update(0,0,0)
	{

		FRIMemoryMap memory;
		memory.MemoryPtr = Memory::Alloc<byte>(1024);
		delegate(memory);

		update = FRIUpdateDescriptor(memory.MemoryPtr, 0, memory.Head);
	}
	void Execute(FRICommandListBase & cmdList);

	~FRICommandStageResources()
	{
		Memory::Free(update.DataArray);
	}
};



//#define ALLOC_COMMAND(...) new ( AllocCommand(sizeof(__VA_ARGS__), alignof(__VA_ARGS__)) ) __VA_ARGS__
//#define ALLOC_COMMAND(...) auto pCmd = new ( AllocCommand(sizeof(__VA_ARGS__), alignof(__VA_ARGS__)) ) __VA_ARGS__
#define ALLOC_COMMAND(...) auto pCmd = new ( AllocCommand(sizeof(__VA_ARGS__), alignof(__VA_ARGS__)) ) __VA_ARGS__
							


class FRICommandList : public FRICommandListBase
{
	bool IsImmediate;

public:

	bool Bypass() const
	{
		return IsImmediate;
	}

	FRICommandList(FRIDynamicAllocator* allocator, bool IsImmediate = true) :
		IsImmediate(IsImmediate)
	{
		this->FriDynamic = allocator;
	}

	FRICommandList(bool IsImmediate = true) : IsImmediate(IsImmediate) {}


	FORCEINLINE void SetViewport(const FViewportRect& viewport)
	{
		if (Bypass())
		{
			GetDynamic()->SetViewport(viewport.X, viewport.Y, viewport.Width, viewport.Height);
			return;
		}
		//ALLOC_COMMAND(FRICommandSetViewport)(viewport.X, viewport.Y, viewport.Width, viewport.Height);

		//pCmd->CommandIdx = NumCommands;
	}


	FORCEINLINE void DrawPrimitives(EFRIPrimitiveType primitiveType, uint32 vertexCount)
	{
		if (Bypass())
		{
			GetDynamic()->DrawPrimitives(primitiveType, vertexCount);
			return;
		}
		//ALLOC_COMMAND(FRICommandDrawPrimitives)(primitiveType, vertexCount);

		//pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void DrawPrimitivesIndexed(EFRIPrimitiveType primitiveType, uint32 vertextCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer)
	{
		if (Bypass())
		{
			GetDynamic()->DrawPrimitivesIndexed(primitiveType, vertextCount, indexType,  indexBuffer);
			return;
		}
		//ALLOC_COMMAND(FRICommandDrawPrimitivesIndexed)(primitiveType, vertextCount, indexType, indexBuffer);

		//pCmd->CommandIdx = NumCommands;
	}




	FORCEINLINE void DrawInstances(EFRIPrimitiveType primitiveType, uint32 vertexCount, uint32 instanceCount)
	{
		if (Bypass())
		{
			GetDynamic()->DrawInstances(primitiveType, vertexCount, instanceCount);
			return;
		}
		//ALLOC_COMMAND(FRICommandDrawInstances)(primitiveType, vertexCount, instanceCount);

		//pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void DrawInstancesIndexed(EFRIPrimitiveType primitiveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer)
	{
		if (Bypass())
		{
			GetDynamic()->DrawInstancesIndexed(primitiveType, vertexCount, instanceCount, indexType, indexBuffer);
			return;
		}
		//ALLOC_COMMAND(FRICommandDrawInstancesIndexed)(primitiveType, vertexCount, instanceCount, indexType, indexBuffer);

	//	pCmd->CommandIdx = NumCommands;

	}







	FORCEINLINE void SetShaderPipeline(FRIShaderPipeline* shaderPipeline)
	{
		if (Bypass())
		{
			GetDynamic()->SetShaderPipeline(shaderPipeline);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetShaderPipeline)(shaderPipeline);

		//pCmd->CommandIdx = NumCommands;

	}
	FORCEINLINE void SetGeometrySource(FRIVertexBuffer* vertexBuffer)
	{
		if (Bypass())
		{
			GetDynamic()->SetGeometrySource(vertexBuffer);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetGeometrySource)(vertexBuffer);

	//	pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void SetInstancedGeometrySource(FRIVertexBuffer* vertexBuffer, FRIInstanceBuffer* instanceBuffer)
	{
		if (Bypass())
		{
			GetDynamic()->SetInstancedGeometrySource(vertexBuffer, instanceBuffer);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetGeometrySource)(vertexBuffer);

	//	pCmd->CommandIdx = NumCommands;

	}


	FORCEINLINE void BindFrameBuffer(FRIFrameBuffer* frameBuffer)
	{
		if (Bypass())
		{
			GetDynamic()->BindFrameBuffer(frameBuffer);
			return;
		}
	//	ALLOC_COMMAND(FRICommandBindFrameBuffer)(frameBuffer);

	//	pCmd->CommandIdx = NumCommands;

	}


	FORCEINLINE void UnbindFrameBuffer()
	{
		if (Bypass())
		{
			GetDynamic()->UnbindFrameBuffer();
			return;
		}
	//	ALLOC_COMMAND(FRICommandUnbindFrameBuffer)();

	//	pCmd->CommandIdx = NumCommands;

	}


	FORCEINLINE void BeginScene()
	{
		if (Bypass())
		{
			GetDynamic()->BeginScene();
			return;
		}
	//	ALLOC_COMMAND(FRICommandBeginScene)();

	//	pCmd->CommandIdx = NumCommands;

	}
	FORCEINLINE void BeginFrame()
	{
		if (Bypass())
		{
			GetDynamic()->BeginFrame();
			return;
		}
	//	ALLOC_COMMAND(FRICommandBeginFrame)();

	//	pCmd->CommandIdx = NumCommands;

	}
	FORCEINLINE void EndScene()
	{
		if (Bypass())
		{
			GetDynamic()->EndScene();
			return;
		}
	//	ALLOC_COMMAND(FRICommandEndScene)();

	//	pCmd->CommandIdx = NumCommands;

	}
	FORCEINLINE void EndFrame()
	{
		if (Bypass())
		{
			GetDynamic()->EndFrame();
			return;
		}
	//	ALLOC_COMMAND(FRICommandEndFrame)();

	//	pCmd->CommandIdx = NumCommands;

	}
	FORCEINLINE void SetShaderUniformBuffer(uint32 slot, FRIUniformBuffer* buffer, uint32 attachFlags = 63)
	{
		if (Bypass())
		{
			GetDynamic()->SetShaderUniformBuffer(slot, buffer, attachFlags);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetShaderUniformBuffer)(slot, buffer);

	//	pCmd->CommandIdx = NumCommands;

	}



	FORCEINLINE void ClearBuffer(FRIFrameBuffer* buffer, Color color)
	{
		if (Bypass())
		{
			GetDynamic()->ClearBuffer(buffer, color);
			return;
		}
	//	ALLOC_COMMAND(FRICommandClearBuffer)(buffer, color);

	//	pCmd->CommandIdx = NumCommands;

	}
	

	FORCEINLINE void SetShaderSampler(FRISampler param)
	{
		if (Bypass())
		{
			GetDynamic()->SetShaderSampler(param);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetShaderSampler)(param);

	//	pCmd->CommandIdx = NumCommands;

	}
	FORCEINLINE void SetTextureParameterBuffer(FRITexture2D* tex, FRITextureParameterBuffer param)
	{
		if (Bypass())
		{
			GetDynamic()->SetTextureParameterBuffer(tex, param);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetTextureParameterBuffer<FRITexture2D>)(tex, param);

	//	pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void SetTextureParameterBuffer(FRITexture2DArray* tex, FRITextureParameterBuffer param)
	{
		if (Bypass())
		{
			GetDynamic()->SetTextureParameterBuffer(tex, param);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetTextureParameterBuffer<FRITexture2DArray>)(tex, param);

	//	pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void SetFramebufferTextureLayer(FRIFrameBuffer* fbo, uint32 layer)
	{
		if (Bypass())
		{
			GetDynamic()->SetFramebufferTextureLayer(fbo, layer);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetFrameBufferTextureLayer)(fbo, layer);

	//	pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void SetRasterizerState(FRIRasterizerState* rasterizer)
	{
		if (Bypass())
		{
			GetDynamic()->SetRasterizerState(rasterizer);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetRasterizerState)(rasterizer);

		//pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void SetBlendState(FRIBlendState* blend)
	{
		if (Bypass())
		{
			GetDynamic()->SetBlendState(blend);
			return;
		}
	//	ALLOC_COMMAND(FRICommandSetBlendState)(blend);

	//	pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void SetDepthStencilState(FRIDepthStencilState* depth)
	{
		if (Bypass())
		{
			GetDynamic()->SetDepthStencilState(depth);
			return;
		}
		//ALLOC_COMMAND(FRICommandSetDepthStencilState)(depth);

	//	pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void FlushMipMaps(FRITexture2D* tex)
	{
		if (Bypass())
		{
			GetDynamic()->FlushMipMaps(tex);
			return;
		}
		//ALLOC_COMMAND(FRICommandFlushMipMaps<FRITexture2D>)(tex);

	//	pCmd->CommandIdx = NumCommands;

	}


	FORCEINLINE void FlushMipMaps(FRITexture2DArray* tex)
	{
		if (Bypass())
		{
			GetDynamic()->FlushMipMaps(tex);
			return;
		}
		//ALLOC_COMMAND(FRICommandFlushMipMaps<FRITexture2DArray>)(tex);

	//	pCmd->CommandIdx = NumCommands;


	}


	FORCEINLINE void FlushMipMaps(FRITexture3D* tex)
	{
		if (Bypass())
		{
			GetDynamic()->FlushMipMaps(tex);
			return;
		}
	//	ALLOC_COMMAND(FRICommandFlushMipMaps<FRITexture3D>)(tex);

	//	pCmd->CommandIdx = NumCommands;

	}

	FORCEINLINE void UniformBufferSubdata(FRIUniformBuffer* buffer, FRIUpdateDescriptor resource)
	{
		if (Bypass())
		{
			GetDynamic()->UniformBufferSubdata(buffer, resource);
			return;
		}
	//	ALLOC_COMMAND(FRICommandUniformBufferSubdata)(buffer, resource);

	//	pCmd->CommandIdx = NumCommands;

	}

	template<typename TLambda>
	FORCEINLINE void StageResourcesLambda(FRIStageBuffer& ubo, TLambda&& lambda)
	{
		if (Bypass())
		{
			ubo.Stage.Head = 0;
			lambda(ubo.Stage);

			GetDynamic()->UniformBufferSubdata(ubo.GPU, FRIUpdateDescriptor(ubo.Stage.MemoryPtr, 0, ubo.Stage.Head));
			return;
		}
	}
	/*

	template<typename TLambda>
	FORCEINLINE void StageResourcesLambda(FRIUniformBuffer* ubo, TLambda&& lambda)
	{

		if (Bypass())
		{
			GetDynamic()->StageResources(ubo, FRIMemoryStageDelegate::Make(lambda));
			return;
		}
		/*else
		{
			FRIMemoryMap memory;
			memory.MemoryPtr = Memory::Alloc<byte>(512);
			lambda(memory);

			FRIUpdateDescriptor update(memory.MemoryPtr, 0, memory.Head);

			ALLOC_COMMAND(FRICommandUniformBufferSubdata)(ubo, update);

			//ALLOC_COMMAND(FRICommandStageResources)(ubo, FRIMemoryStageDelegate::Make(lambda));
			pCmd->CommandIdx = NumCommands;
		}

		ALLOC_COMMAND(FRICommandStageResources)(ubo, FRIMemoryStageDelegate::Make(lambda));
		pCmd->CommandIdx = NumCommands;

	}

	template<typename TStageableResource>
	FORCEINLINE void StageResource(FRIUniformBuffer* ubo, const TStageableResource& resource)
	{
		if (Bypass())
		{
			GetDynamic()->StageResources(ubo, FRIMemoryStageDelegate::Make([&](FRIMemoryMap& mem) {
				resource.StageMemory(mem);
				}));

			return;
		}
		/*else
		{
			FRIMemoryMap memory;
			memory.MemoryPtr = Memory::Alloc<byte>(512);
			resource.StageMemory(memory);

			FRIUpdateDescriptor update(memory.MemoryPtr, 0, memory.Head);

			ALLOC_COMMAND(FRICommandUniformBufferSubdata)(ubo, update);
			pCmd->CommandIdx = NumCommands;

		}
		ALLOC_COMMAND(FRICommandStageResources)(ubo, FRIMemoryStageDelegate::Make([&](FRIMemoryMap& mem) {
			resource.StageMemory(mem);
			}));


	}*/
};





#include "FRICommandList.inl"