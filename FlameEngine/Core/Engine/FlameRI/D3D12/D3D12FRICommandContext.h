#pragma once


class D3D12FRICommandContext : public FRICommandContext, FD3D12DeviceChild
{
protected:
	friend class FD3D12Device;
	friend class FD3D12VertexBuffer;
	friend class FD3D12IndexBuffer;

	ComPtr<ID3D12GraphicsCommandList> CommandList = nullptr;
	FArray<ID3D12CommandAllocator*> CommandAllocators;

	FArray<FD3D12RenderTargetView*> BoundRenderTargets;
	FD3D12DepthStencilView* BoundDSV;

	FArray<FD3D12Resource*> RenderTargets;
	FD3D12Resource* DSV;

	FArray<ID3D12Fence*> Fences;
	FArray<uint64> FenceValues;
	uint32 BackBufferIndex;
	HANDLE FenceEvent;

	FD3D12FrameCache FrameCache;
	
	int RTVDescriptorSize = 0;
public:

	ComPtr<ID3D12GraphicsCommandList> GetCommandListHandle() { return CommandList; }

	D3D12FRICommandContext(FD3D12Device* Parent);


	void ResourceSubdata(FRIResourceObject* buffer, FRIUpdateDescriptor resource);
	void ClearRenderTarget(FRIRenderTargetView* buffer, Color32 color, FRIUnorderedAccessView** uavs = NULL, uint32 uavNum = 0);
	void ClearDepthStencil(FRIDepthStencilView* dsv, float depth);
	void GenerateMipMaps(FRIShaderResourceView* resource);
	void CopyResource(FRIResourceObject* source, FRIResourceObject* dest);

	// Binding calls
	void SetGeometrySource(FRIVertexBuffer* vertexBuffer, FRIIndexBuffer* indexBuffer, FRIInstanceBuffer* instanceBuffer = nullptr);
	void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height);
	void BindRenderTargets(uint32 Count, FRIRenderTargetView** renderTargets, FRIDepthStencilView* DSV);

	void SetShaderSamplerVS(uint32 slot, FRISamplerState* sampler);
	void SetShaderSamplerPS(uint32 slot, FRISamplerState* sampler);
	void SetShaderSamplerGS(uint32 slot, FRISamplerState* sampler);
	void SetShaderSamplerHS(uint32 slot, FRISamplerState* sampler);
	void SetShaderSamplerDS(uint32 slot, FRISamplerState* sampler);
	void SetShaderSamplerCS(uint32 slot, FRISamplerState* sampler);

	void SetShaderResourceViewVS(uint32 slot, FRIShaderResourceView* resource);
	void SetShaderResourceViewPS(uint32 slot, FRIShaderResourceView* resource);
	void SetShaderResourceViewHS(uint32 slot, FRIShaderResourceView* resource);
	void SetShaderResourceViewDS(uint32 slot, FRIShaderResourceView* resource);
	void SetShaderResourceViewGS(uint32 slot, FRIShaderResourceView* resource);
	void SetShaderResourceViewCS(uint32 slot, FRIShaderResourceView* resource);

	void SetUnorderedAccessViewPS(uint32 slot, FRIUnorderedAccessView* buffer);
	void SetUnorderedAccessViewCS(uint32 slot, FRIUnorderedAccessView* buffer);

	void SetConstantBufferViewVS(uint32 slot, FRIConstantBufferView* buffer);
	void SetConstantBufferViewPS(uint32 slot, FRIConstantBufferView* buffer);
	void SetConstantBufferViewHS(uint32 slot, FRIConstantBufferView* buffer);
	void SetConstantBufferViewDS(uint32 slot, FRIConstantBufferView* buffer);
	void SetConstantBufferViewGS(uint32 slot, FRIConstantBufferView* buffer);
	void SetConstantBufferViewCS(uint32 slot, FRIConstantBufferView* buffer);

	void SetPipelineState(FRIPipelineStateObject* shader);

	void OpenCommandList();
	void CloseCommandList();

	// Draw Calls
	void DrawInstances(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount);
	void DrawInstancesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType);
	void DrawPrimitives(EFRIPrimitiveType primitveType, uint32 vertexCount);
	void DrawPrimitivesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, EFRIIndexType indexType);


	// Compute Calls
	void DispatchCompute(
		uint32 xThreads,
		uint32 yThreads,
		uint32 zThreads
	);

	void BeginCompute();
	void EndCompute();

	// Frame

	void BeginFrame();
	void EndFrame();

	void WaitForPreviousFrame();

};
