#include "FRICommandList.h"


#define IMPL_CMD(cmd) void FRICommand##cmd::Execute(FRICommandListBase& cmdList)

#define ALLOCATOR cmdList.FriDynamic


IMPL_CMD(SetViewport)
{
	ALLOCATOR->SetViewport(x, y, width, height);
}

IMPL_CMD(BindFrameBuffer)
{
	ALLOCATOR->BindFrameBuffer(frameBuffer);
}

IMPL_CMD(SetGeometrySource)
{
	ALLOCATOR->SetGeometrySource(vertexBuffer);
}
IMPL_CMD(DrawPrimitives)
{
	ALLOCATOR->DrawPrimitives(elementType, elementCount);
}
IMPL_CMD(DrawPrimitivesIndexed)
{
	ALLOCATOR->DrawPrimitivesIndexed(elementType, elementCount, indexType, indexBuffer);
}
IMPL_CMD(SetShaderPipeline)
{
	ALLOCATOR->SetShaderPipeline(shaderPipelineProgram);
}

IMPL_CMD(BeginFrame)
{
	ALLOCATOR->BeginFrame();
}

IMPL_CMD(EndFrame)
{
	ALLOCATOR->EndFrame();
}
