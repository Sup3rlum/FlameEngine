#include "FRICommandList.h"


#define IMPL_CMD(cmd) void FRICommand##cmd::Execute(FRICommandListBase& cmdList)

#define ALLOCATOR cmdList.GetContext()


void FRICommandSetViewport::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.SetViewport(x, y, width, height);
}

void FRICommandBindRenderTargets::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.BindRenderTargets(renderTargets.Length(), renderTargets.Begin(), NULL);
}

void FRICommandSetGeometrySource::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.SetGeometrySource(vertexBuffer, indexBuffer);
}
void FRICommandDrawPrimitives::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.DrawPrimitives(primitiveType, vertexCount);
}
void FRICommandDrawPrimitivesIndexed::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.DrawPrimitivesIndexed(primitiveType, indexCount, indexType);
}
void FRICommandSetShaderPipeline::Execute(FRICommandListBase& cmdList)
{
	//ALLOCATOR.SetShaderPipeline(shader);
}

void FRICommandBeginFrame::Execute(FRICommandListBase& cmdList)
{
	//ALLOCATOR.BeginFrame();
}

void FRICommandEndFrame::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.EndFrame();
}

void FRICommandClearRenderTarget::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.ClearRenderTarget(renderTarget, clearColor);
}

void FRICommandDrawInstancesIndexed::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.DrawInstancesIndexed(primitiveType, indexCount, instanceCount, indexType);
}

void FRICommandSetShaderConstantBuffer::Execute(FRICommandListBase& cmdList)
{
//	ALLOCATOR.SetShaderConstantBuffer(slot, uniformBuffer);
}
void FRICommandUnbindFrameBuffer::Execute(FRICommandListBase& cmdList)
{
//	ALLOCATOR.UnbindFrameBuffer();
}

void FRICommandSetDepthStencilState::Execute(FRICommandListBase& cmdList)
{
	//ALLOCATOR.SetDepthStencilState(depth);
}

void FRICommandSetRasterizerState::Execute(FRICommandListBase& cmdList)
{
	//ALLOCATOR.SetRasterizerState(rasterizer);
}
void FRICommandSetBlendState::Execute(FRICommandListBase& cmdList)
{
	//ALLOCATOR.SetBlendState(blend);
}

void FRICommandSetShaderSamplerState::Execute(FRICommandListBase& cmdList)
{
	//ALLOCATOR.SetShaderSamplerState(slot, sampler);
}

void FRICommandSetShaderResourceView::Execute(FRICommandListBase& cmdList)
{
	//ALLOCATOR.SetShaderResource(slot, resource);
}

void FRICommandResourceSubdata::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.ResourceSubdata(resource, data);
}

void FRICommandStageResources::Execute(FRICommandListBase& cmdList)
{
	ALLOCATOR.ResourceSubdata(buffer, update);
}

