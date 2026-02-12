#pragma once

class FD3D12RootSignature;
class FD3D12ShaderResourceView;
class FD3D12ConstantBufferView;
class FD3D12UnorderedAccessView;
class FD3D12RenderTargetView;
class FD3D12SamplerState;
class FD3D12PipelineStateObject;

class FD3D12DescriptorCache : public FD3D12DeviceChild
{
public:
	FD3D12DescriptorCache(FD3D12Device* InDevice) :
		FD3D12DeviceChild(InDevice),
		ViewHeap(new FD3D12OnlineDescriptorHeap(InDevice, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 512)),
		SamplerHeap(new FD3D12OnlineDescriptorHeap(InDevice, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 512))
	{
		ViewHeap->GetHeap()->SetName(L"Cache View Heap");
		SamplerHeap->GetHeap()->SetName(L"Cache Sampler Heap");
	}

	void InitFrame(ID3D12GraphicsCommandList* CommandList);

	template<EShaderFrequency ShaderFrequency>
	void SetShaderResourceViews(ID3D12GraphicsCommandList* CommandList, FD3D12RootSignature* RootSignature, uint32 NumSlots, FD3D12ShaderResourceView* SRVs[], uint32& FirstHeapSlot);

	template<EShaderFrequency ShaderFrequency>
	void SetConstantBufferViews(ID3D12GraphicsCommandList* CommandList, FD3D12RootSignature* RootSignature, uint32 NumSlots, FD3D12ConstantBufferView* CBVs[], uint32& FirstHeapSlot);

	template<EShaderFrequency ShaderFrequency>
	void SetUnorderedAccessViews(ID3D12GraphicsCommandList* CommandList, FD3D12RootSignature* RootSignature, uint32 NumSlots, FD3D12UnorderedAccessView* UAVs[], uint32& FirstHeapSlot);

	template<EShaderFrequency ShaderFrequency>
	void SetSamplers(ID3D12GraphicsCommandList* CommandList, FD3D12RootSignature* RootSignature, uint32 NumSlots, FD3D12SamplerState* Samplers[]);


	FD3D12OnlineDescriptorHeap* ViewHeap;
	FD3D12OnlineDescriptorHeap* SamplerHeap;
};


class FD3D12FrameCache : public FD3D12DeviceChild
{
	FD3D12DescriptorCache DescriptorCache;
	FD3D12ShaderResourceView* BoundSRVs[SF_ShaderStages][16];
	FD3D12ConstantBufferView* BoundCBVs[SF_ShaderStages][16];
	FD3D12SamplerState* BoundSamplers[SF_ShaderStages][16];
	FD3D12UnorderedAccessView* BoundUAVs[2][16];
	FD3D12PipelineStateObject* BoundPSO;

	FD3D12RenderTargetView* BoundRTVs[8];

	uint64 DirtySRVMask[SF_ShaderStages] = { 0 };
	uint64 DirtyCBVMask[SF_ShaderStages] = { 0 };
	uint64 DirtySamplerMask[SF_ShaderStages] = { 0 };
	uint64 DirtyUAVMask[2] = { 0 };

	bool NeedSetViewports = false;
	bool NeedSetPrimitiveTopology = false;
	bool NeedSetVBs = false;
	bool NeedSetIB = false;
	bool NeedSetRTs = false;

public:

	FD3D12FrameCache(FD3D12Device* Parent) : FD3D12DeviceChild(Parent), DescriptorCache(Parent) {}

	template<EShaderFrequency ShaderFrequency>
	void SetShaderResourceView(uint32 Slot, FRIShaderResourceView* SRV)
	{
		BoundSRVs[ShaderFrequency][Slot] = DX12::Cast<FD3D12ShaderResourceView>(SRV);
		DirtySRVMask[ShaderFrequency] |= (1l << Slot);
	}

	template<EShaderFrequency ShaderFrequency>
	void SetConstantBufferView(uint32 Slot, FRIConstantBufferView* CBV)
	{
		BoundCBVs[ShaderFrequency][Slot] = DX12::Cast<FD3D12ConstantBufferView>(CBV);
		DirtyCBVMask[ShaderFrequency] |= (1l << Slot);
	}

	void SetUnorderedAccessViewPS(uint32 Slot, FRIUnorderedAccessView* UAV)
	{
		BoundUAVs[0][Slot] = DX12::Cast<FD3D12UnorderedAccessView>(UAV);
		DirtyUAVMask[0] |= (1l << Slot);
	}
	void SetUnorderedAccessViewCS(uint32 Slot, FRIUnorderedAccessView* UAV)
	{
		BoundUAVs[1][Slot] = DX12::Cast<FD3D12UnorderedAccessView>(UAV);
		DirtyUAVMask[1] |= (1l << Slot);
	}

	template<EShaderFrequency ShaderFrequency>
	void SetSampler(uint32 Slot, FRISamplerState* Sampler)
	{
		BoundSamplers[ShaderFrequency][Slot] = DX12::Cast<FD3D12SamplerState>(Sampler);
		DirtySamplerMask[1] |= (1l << Slot);
	}

	void SetPSO(FD3D12PipelineStateObject* PSO)
	{
		BoundPSO = PSO;
	}

	void InitFrame(ID3D12GraphicsCommandList* CommandList)
	{
		DescriptorCache.InitFrame(CommandList);
		FirstHeapSlot = 0;
	}

	void ApplySamplers(ID3D12GraphicsCommandList* CommandList);
	void ApplySRVs(ID3D12GraphicsCommandList* CommandList);
	void ApplyUAVs(ID3D12GraphicsCommandList* CommandList);
	void ApplyCBVs(ID3D12GraphicsCommandList* CommandList);

	void ApplyState(ID3D12GraphicsCommandList* CommandList);
	void ApplyComputeState(ID3D12GraphicsCommandList* CommandList);

	uint32 FirstHeapSlot = 0;
};