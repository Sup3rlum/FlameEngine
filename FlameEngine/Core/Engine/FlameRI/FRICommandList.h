#pragma once

#include "FRIResource.h"
#include "FRIContext.h"
#include "FRICommandContext.h"
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


#define RegisterFRICmd(cmdName) struct FRICommand##cmdName : FRICommand<FRICommand##cmdName>
#define RegisterFRICmdMulti(cmdName, ...) template<CALL_MACRO_X_FOR_EACH(FTEMPLATETYPENAME, __VA_ARGS__)> \
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
		Context(0),
		ComputeContext(0)
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

	FRICommandContext& GetContext()
	{
		return *Context;
	}
	void Flush();

	FMemoryStack MemoryStack;
	FRICommandBase* First;
	FRICommandBase** CommandLink;
	uint32 NumCommands;

	FRICommandContext* Context;
	FRICommandContext* ComputeContext;
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

RegisterFRICmd(SetViewport)
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


RegisterFRICmd(BindRenderTargets)
{
	FArray<FRIRenderTargetView*> renderTargets;

	FRICmdInit(BindRenderTargets)(FArray<FRIRenderTargetView*> renderTargets) :
		renderTargets(renderTargets)
	{
	}

	void Execute(FRICommandListBase& cmdList);

};

RegisterFRICmd(UnbindFrameBuffer)
{
	FRICmdInit(UnbindFrameBuffer)()
	{
	}

	void Execute(FRICommandListBase & cmdList);

};


RegisterFRICmd(ClearRenderTarget)
{
	FRIRenderTargetView* renderTarget;
	Color clearColor;

	FRICmdInit(ClearRenderTarget)(FRIRenderTargetView* renderTarget, Color clearColor) :
		renderTarget(renderTarget),
		clearColor(clearColor)
	{

	}

	void Execute(FRICommandListBase& cmdList);

};


RegisterFRICmd(BeginFrame)
{
	FRICmdInit(BeginFrame)()
	{

	}
	void Execute(FRICommandListBase& cmdList);
};


RegisterFRICmd(EndFrame)
{
	FRICmdInit(EndFrame)()
	{

	}
	void Execute(FRICommandListBase& cmdList);
};

RegisterFRICmd(BeginScene)
{
	FRICmdInit(BeginScene)()
	{

	}
	void Execute(FRICommandListBase& cmdList);
};

RegisterFRICmd(EndScene)
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


RegisterFRICmd(SetGeometrySource)
{
	FRIVertexBuffer* vertexBuffer;
	FRIIndexBuffer* indexBuffer;
	FRIInstanceBuffer* instanceBuffer;

	FRICmdInit(SetGeometrySource)(FRIVertexBuffer* vertexBuffer, FRIIndexBuffer* indexBuffer, FRIInstanceBuffer* instanceBuffer) :
		vertexBuffer(vertexBuffer),
		indexBuffer(indexBuffer),
		instanceBuffer(instanceBuffer)
	{

	}
	void Execute(FRICommandListBase& cmdList);

};



RegisterFRICmd(DrawPrimitives)
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

RegisterFRICmd(DrawPrimitivesIndexed)
{

	EFRIPrimitiveType primitiveType;
	uint32 indexCount;
	EFRIIndexType indexType;
	FRIIndexBuffer* indexBuffer;

	FRICmdInit(DrawPrimitivesIndexed)(EFRIPrimitiveType primitiveType, uint32 indexCount, EFRIIndexType indexType) :
		primitiveType(primitiveType),
		indexCount(indexCount),
		indexType(indexType)
	{

	}

	void Execute(FRICommandListBase& cmdList);

};



RegisterFRICmd(DrawInstances)
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

RegisterFRICmd(DrawInstancesIndexed)
{

	EFRIPrimitiveType primitiveType;
	uint32 indexCount;
	uint32 instanceCount;
	EFRIIndexType indexType;
	FRIIndexBuffer* indexBuffer;

	FRICmdInit(DrawInstancesIndexed)(EFRIPrimitiveType primitiveType, uint32 indexCount, uint32 instanceCount, EFRIIndexType indexType) :
		primitiveType(primitiveType),
		indexCount(indexCount),
		instanceCount(instanceCount),
		indexType(indexType)
	{

	}

	void Execute(FRICommandListBase & cmdList);

};


/*
* 
* SHADERS
* 
*/



RegisterFRICmd(SetShaderPipeline)
{
	FRIShaderPipeline* shader;

	FRICmdInit(SetShaderPipeline)(FRIShaderPipeline* shader) :
		shader(shader)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


RegisterFRICmd(SetShaderConstantBuffer)
{
	uint32 slot;
	FRIConstantBuffer* uniformBuffer;

	FRICmdInit(SetShaderConstantBuffer)(uint32 slot, FRIConstantBuffer* uniformBuffer) :
		slot(slot),
		uniformBuffer(uniformBuffer)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};


RegisterFRICmd(SetShaderResourceView)
{
	uint32 slot;
	FRIShaderResourceView* resource;

	FRICmdInit(SetShaderResourceView)(uint32 slot, FRIShaderResourceView* resource) :
		slot(slot),
		resource(resource)
	{

	}

	void Execute(FRICommandListBase& cmdList);
};

RegisterFRICmd(SetShaderSamplerState)
{
	uint32 slot;
	FRISamplerState* sampler;

	FRICmdInit(SetShaderSamplerState)(uint32 slot, FRISamplerState* sampler) :
		slot(slot),
		sampler(sampler)
	{

	}

	void Execute(FRICommandListBase & cmdList);
};

/*
* 
* TEXTURES
* 
*/


RegisterFRICmd(FlushMipMaps)
{
	FRIShaderResourceView* resource;

	FRICmdInit(FlushMipMaps)(FRIShaderResourceView* resource) :
		resource(resource)
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




RegisterFRICmd(SetRasterizerState)
{
	FRIRasterizerState* rasterizer;
	
	FRICmdInit(SetRasterizerState)(FRIRasterizerState * rasterizer) :
		rasterizer(rasterizer)
	{

	}
	void Execute(FRICommandListBase& cmdList);
};


RegisterFRICmd(SetBlendState)
{
	FRIBlendState* blend;

	FRICmdInit(SetBlendState)(FRIBlendState * blend) :
		blend(blend)
	{

	}
	void Execute(FRICommandListBase & cmdList);
};

RegisterFRICmd(SetDepthStencilState)
{
	FRIDepthStencilState* depth;

	FRICmdInit(SetDepthStencilState)(FRIDepthStencilState* depth) :
		depth(depth)
	{

	}
	void Execute(FRICommandListBase & cmdList);
};

RegisterFRICmd(ResourceSubdata)
{
	FRIResourceObject* resource;
	FRIUpdateDescriptor data;

	FRICmdInit(ResourceSubdata)(FRIResourceObject* resource, FRIUpdateDescriptor desc) :
		resource(resource),
		data(desc)
	{

	}
	void Execute(FRICommandListBase & cmdList);
};

RegisterFRICmd(StageResources)
{
	FRIConstantBuffer* buffer;
	FRIMemoryStageDelegate delegate;
	byte* DataOwner;

	FRIUpdateDescriptor update;

	FRICmdInit(StageResources)(FRIConstantBuffer* buffer, FRIMemoryStageDelegate delegate) :
		buffer(buffer),
		delegate(delegate),
		update(0,0,0)
	{

		FRIMemoryMap memory;
		DataOwner = Memory::Alloc<byte>(1024);
		memory.MemoryPtr = DataOwner;
		delegate(memory);

		update = FRIUpdateDescriptor(memory.MemoryPtr, 0, memory.Head);
	}
	void Execute(FRICommandListBase & cmdList);

	~FRICommandStageResources()
	{
		Memory::Free(DataOwner);
	}
};



//#define ALLOC_COMMAND(...) new ( AllocCommand(sizeof(__VA_ARGS__), alignof(__VA_ARGS__)) ) __VA_ARGS__
//#define ALLOC_COMMAND(...) auto pCmd = new ( AllocCommand(sizeof(__VA_ARGS__), alignof(__VA_ARGS__)) ) __VA_ARGS__
#define ALLOC_COMMAND(...) auto pCmd = new ( AllocCommand(sizeof(__VA_ARGS__), alignof(__VA_ARGS__)) ) __VA_ARGS__ 						

template<bool ExecuteOnFRIThread>
class FLAME_API TFRICommandList : public FRICommandListBase
{
public:

	TFRICommandList(FRICommandContext* commandContext)
	{
		Context = commandContext;
	}

	FORCEINLINE void SetViewport(const FViewportRect& viewport)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetViewport(viewport.X, viewport.Y, viewport.Width, viewport.Height);
			return;
		}
		ALLOC_COMMAND(FRICommandSetViewport)(viewport.X, viewport.Y, viewport.Width, viewport.Height);
	}


	FORCEINLINE void DrawPrimitives(EFRIPrimitiveType primitiveType, uint32 vertexCount)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().DrawPrimitives(primitiveType, vertexCount);
			return;
		}
		ALLOC_COMMAND(FRICommandDrawPrimitives)(primitiveType, vertexCount);
	}

	FORCEINLINE void DrawPrimitivesIndexed(EFRIPrimitiveType primitiveType, uint32 vertextCount, EFRIIndexType indexType)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().DrawPrimitivesIndexed(primitiveType, vertextCount, indexType);
			return;
		}
		ALLOC_COMMAND(FRICommandDrawPrimitivesIndexed)(primitiveType, vertextCount, indexType);
	}

	FORCEINLINE void DrawInstances(EFRIPrimitiveType primitiveType, uint32 vertexCount, uint32 instanceCount)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().DrawInstances(primitiveType, vertexCount, instanceCount);
			return;
		}
		ALLOC_COMMAND(FRICommandDrawInstances)(primitiveType, vertexCount, instanceCount);
	}

	FORCEINLINE void DrawInstancesIndexed(EFRIPrimitiveType primitiveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().DrawInstancesIndexed(primitiveType, vertexCount, instanceCount, indexType);
			return;
		}
		ALLOC_COMMAND(FRICommandDrawInstancesIndexed)(primitiveType, vertexCount, instanceCount, indexType);
	}

	FORCEINLINE void DispatchCompute(uint32 x, uint32 y, uint32 z)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().DispatchCompute(x, y, z);
			return;
		}
	}

	FORCEINLINE void SetShaderPipeline(FRIShaderPipeline* shaderPipeline)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			//GetContext().SetShaderPipeline(shaderPipeline);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderPipeline)(shaderPipeline);
	}


	FORCEINLINE void SetGeometrySource(FRIVertexBuffer* vertexBuffer, FRIIndexBuffer* indexBuffer, FRIInstanceBuffer* instanceBuffer = nullptr)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetGeometrySource(vertexBuffer, indexBuffer, instanceBuffer);
			return;
		}
		ALLOC_COMMAND(FRICommandSetGeometrySource)(vertexBuffer, indexBuffer, instanceBuffer);
	}


	FORCEINLINE void BindRenderTargets(const FArray<FRIRenderTargetView*>& renderTargetViews, FRIDepthStencilView* DSV)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().BindRenderTargets(renderTargetViews.Length(), renderTargetViews.Begin(), DSV);
			return;
		}
		ALLOC_COMMAND(FRICommandBindRenderTargets)(renderTargetViews);
	}


	FORCEINLINE void UnbindFrameBuffer()
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().UnbindFrameBuffer();
			return;
		}
		ALLOC_COMMAND(FRICommandUnbindFrameBuffer)();
	}


	FORCEINLINE void BeginScene()
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().BeginScene();
			return;
		}
		ALLOC_COMMAND(FRICommandBeginScene)();
	}
	FORCEINLINE void BeginFrame()
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().BeginFrame();
			return;
		}
		ALLOC_COMMAND(FRICommandBeginFrame)();
	}
	FORCEINLINE void EndScene()
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().EndScene();
			return;
		}
		ALLOC_COMMAND(FRICommandEndScene)();
	}
	FORCEINLINE void EndFrame()
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().EndFrame();
			return;
		}
		ALLOC_COMMAND(FRICommandEndFrame)();
	}

	FORCEINLINE void BeginCompute()
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().BeginCompute();
			return;
		}
		//ALLOC_COMMAND(FRICommandBeginFrame)();
	}
	FORCEINLINE void EndCompute()
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().EndCompute();
			return;
		}
		//ALLOC_COMMAND(FRICommandBeginFrame)();
	}

	FORCEINLINE void SetShaderConstantBuffer(uint32 slot, FRIConstantBuffer* buffer, uint32 attachFlags = 63)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			//GetContext().SetShaderConstantBuffer(slot, buffer, attachFlags);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderConstantBuffer)(slot, buffer);
	}



	FORCEINLINE void ClearRenderTarget(FRIRenderTargetView* renderTarget, Color color)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().ClearRenderTarget(renderTarget, color);
			return;
		}
		ALLOC_COMMAND(FRICommandClearRenderTarget)(renderTarget, color);
	}

	FORCEINLINE void ClearDepthStencil(FRIDepthStencilView* depthTarget, float value)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().ClearDepthStencil(depthTarget, value);
			return;
		}
		//ALLOC_COMMAND(FRICommandClearRenderTarget)(renderTarget, color);
	}
	
	/* Set Shader Resource View */

	FORCEINLINE void SetShaderResourceViewVS(uint32 slot, FRIShaderResourceView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderResourceViewVS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderResourceView)(slot, param);
	}

	FORCEINLINE void SetShaderResourceViewPS(uint32 slot, FRIShaderResourceView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderResourceViewPS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderResourceView)(slot, param);
	}

	FORCEINLINE void SetShaderResourceViewGS(uint32 slot, FRIShaderResourceView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderResourceViewGS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderResourceView)(slot, param);
	}

	FORCEINLINE void SetShaderResourceViewHS(uint32 slot, FRIShaderResourceView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderResourceViewHS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderResourceView)(slot, param);
	}

	FORCEINLINE void SetShaderResourceViewDS(uint32 slot, FRIShaderResourceView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderResourceViewDS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderResourceView)(slot, param);
	}

	FORCEINLINE void SetShaderResourceViewCS(uint32 slot, FRIShaderResourceView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderResourceViewCS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderResourceView)(slot, param);
	}

	/* Set Shader Sampler */

	FORCEINLINE void SetShaderSamplerVS(uint32 slot, FRISamplerState* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderSamplerVS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}

	FORCEINLINE void SetShaderSamplerPS(uint32 slot, FRISamplerState* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderSamplerPS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}

	FORCEINLINE void SetShaderSamplerGS(uint32 slot, FRISamplerState* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderSamplerGS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}

	FORCEINLINE void SetShaderSamplerHS(uint32 slot, FRISamplerState* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderSamplerHS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}

	FORCEINLINE void SetShaderSamplerDS(uint32 slot, FRISamplerState* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetShaderSamplerDS(slot, param);
			return;
		}
		ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}

	// CBVs

	FORCEINLINE void SetConstantBufferViewVS(uint32 slot, FRIConstantBufferView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetConstantBufferViewVS(slot, param);
			return;
		}
		//ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}
	FORCEINLINE void SetConstantBufferViewPS(uint32 slot, FRIConstantBufferView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetConstantBufferViewPS(slot, param);
			return;
		}
		//ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}
	FORCEINLINE void SetConstantBufferViewCS(uint32 slot, FRIConstantBufferView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetConstantBufferViewCS(slot, param);
			return;
		}
		//ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}

	// UAVs

	FORCEINLINE void SetUnorderedAccessViewCS(uint32 slot, FRIUnorderedAccessView* param)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetUnorderedAccessViewCS(slot, param);
			return;
		}
		//ALLOC_COMMAND(FRICommandSetShaderSamplerState)(slot, param);
	}

	// Other Stuff

	FORCEINLINE void SetRasterizerState(FRIRasterizerState* rasterizer)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			//GetContext().SetRasterizerState(rasterizer);
			return;
		}
		ALLOC_COMMAND(FRICommandSetRasterizerState)(rasterizer);
	}

	FORCEINLINE void SetBlendState(FRIBlendState* blend)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			//GetContext().SetBlendState(blend);
			return;
		}
		ALLOC_COMMAND(FRICommandSetBlendState)(blend);
	}

	FORCEINLINE void SetDepthStencilState(FRIDepthStencilState* depth)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			//GetContext().SetDepthStencilState(depth);
			return;
		}
		ALLOC_COMMAND(FRICommandSetDepthStencilState)(depth);
	}

	FORCEINLINE void FlushMipMaps(FRIShaderResourceView* tex)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().FlushMipMaps(tex);
			return;
		}
		ALLOC_COMMAND(FRICommandFlushMipMaps)(tex);
	}


	FORCEINLINE void ResourceSubdata(FRIResourceObject* resource, FRIUpdateDescriptor data)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().ResourceSubdata(resource, data);
			return;
		}
		ALLOC_COMMAND(FRICommandResourceSubdata)(resource, data);
	}

	FORCEINLINE void SetPipelineState(FRIPipelineStateObject* pipelineState)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			GetContext().SetPipelineState(pipelineState);
			return;
		}
		//ALLOC_COMMAND(FRICommandResourceSubdata)(resource, data);
	}

	FORCEINLINE void UpdateSubresources(FRIConstantBuffer* resource)
	{
		if constexpr (ExecuteOnFRIThread)
		{
			return;
		}
	}

	template<typename TLambda>
	FORCEINLINE void StageResources(FRIConstantBuffer* Buffer, TLambda lambda)
	{
		FRIMemoryMap Map;
		lambda(Map);

		auto Update = FRIUpdateDescriptor(Map.MemoryPtr, 0, Buffer->ByteSize);
		GetContext().ResourceSubdata(Buffer, Update);
	}

	FORCEINLINE void Open()
	{
		Context->OpenCommandList();
	}

	FORCEINLINE void CloseAndExecute()
	{
		Context->CloseCommandList();
	}

};


typedef TFRICommandList<true> FRICommandList;
typedef TFRICommandList<false> FRITaskCommandList;

#include "FRICommandList.inl"