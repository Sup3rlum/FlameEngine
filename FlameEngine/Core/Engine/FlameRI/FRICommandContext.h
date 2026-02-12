#pragma once


#include "Core/Common/CoreCommon.h"
#include "FRIResource.h"
#include "ShaderLibrary/ShaderLibrary.h"



class FRICommandContext
{
public:

	// Resource calls

	virtual void ResourceSubdata(FRIResourceObject* buffer, FRIUpdateDescriptor resource) = 0;
	virtual void ClearRenderTarget(FRIRenderTargetView* buffer, Color32 color, FRIUnorderedAccessView** uavs = NULL, uint32 uavNum = 0) = 0;
	virtual void ClearDepthStencil(FRIDepthStencilView* dsv, float depth) = 0;
	virtual void GenerateMipMaps(FRIShaderResourceView* resource) = 0;
	virtual void CopyResource(FRIResourceObject* source, FRIResourceObject* dest) = 0;

	// Binding calls
	virtual void SetGeometrySource(FRIVertexBuffer* vertexBuffer, FRIIndexBuffer* indexBuffer, FRIInstanceBuffer* instanceBuffer = nullptr) = 0;
	virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
	virtual void BindRenderTargets(uint32 Count, FRIRenderTargetView** renderTargets, FRIDepthStencilView* DSV) = 0;

	virtual void SetShaderSamplerVS(uint32 slot, FRISamplerState* sampler) = 0;
	virtual void SetShaderSamplerPS(uint32 slot, FRISamplerState* sampler) = 0;
	virtual void SetShaderSamplerGS(uint32 slot, FRISamplerState* sampler) = 0;
	virtual void SetShaderSamplerHS(uint32 slot, FRISamplerState* sampler) = 0;
	virtual void SetShaderSamplerDS(uint32 slot, FRISamplerState* sampler) = 0;
	virtual void SetShaderSamplerCS(uint32 slot, FRISamplerState* sampler) = 0;

	virtual void SetShaderResourceViewVS(uint32 slot, FRIShaderResourceView* resource) = 0;
	virtual void SetShaderResourceViewPS(uint32 slot, FRIShaderResourceView* resource) = 0;
	virtual void SetShaderResourceViewHS(uint32 slot, FRIShaderResourceView* resource) = 0;
	virtual void SetShaderResourceViewDS(uint32 slot, FRIShaderResourceView* resource) = 0;
	virtual void SetShaderResourceViewGS(uint32 slot, FRIShaderResourceView* resource) = 0;
	virtual void SetShaderResourceViewCS(uint32 slot, FRIShaderResourceView* resource) = 0;

	virtual void SetUnorderedAccessViewPS(uint32 slot, FRIUnorderedAccessView* buffer) = 0;
	virtual void SetUnorderedAccessViewCS(uint32 slot, FRIUnorderedAccessView* buffer) = 0;

	virtual void SetConstantBufferViewVS(uint32 slot, FRIConstantBufferView* buffer) = 0;
	virtual void SetConstantBufferViewPS(uint32 slot, FRIConstantBufferView* buffer) = 0;
	virtual void SetConstantBufferViewHS(uint32 slot, FRIConstantBufferView* buffer) = 0;
	virtual void SetConstantBufferViewDS(uint32 slot, FRIConstantBufferView* buffer) = 0;
	virtual void SetConstantBufferViewGS(uint32 slot, FRIConstantBufferView* buffer) = 0;
	virtual void SetConstantBufferViewCS(uint32 slot, FRIConstantBufferView* buffer) = 0;

	virtual void SetPipelineState(FRIPipelineStateObject* shader) = 0;

	virtual void OpenCommandList() = 0;
	virtual void CloseCommandList() = 0;

	// Draw Calls
	virtual void DrawInstances(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount) = 0;
	virtual void DrawInstancesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType) = 0;
	virtual void DrawPrimitives(EFRIPrimitiveType primitveType, uint32 vertexCount) = 0;
	virtual void DrawPrimitivesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, EFRIIndexType indexType) = 0;

	// Compute Calls
	virtual void DispatchCompute(
		uint32 xThreads,
		uint32 yThreads,
		uint32 zThreads
	) = 0;

	// Frame

	virtual void EndFrame() = 0;
	virtual void BeginFrame() = 0;

	virtual void EndCompute() = 0;
	virtual void BeginCompute() = 0;
};