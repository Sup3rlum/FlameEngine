#include "D3D12FRICommon.h"


template<EShaderFrequency ShaderFrequency>
void FD3D12DescriptorCache::SetShaderResourceViews(ID3D12GraphicsCommandList* CommandList, FD3D12RootSignature* RootSignature, uint32 NumSlots, FD3D12ShaderResourceView* SRVs[], uint32& HeapSlot)
{
	uint32 SlotsNeeded = 0;
	for (uint32 SlotIdx = 0; SlotIdx < NumSlots; SlotIdx++)
	{
		if (SRVs[SlotIdx])
			SlotsNeeded = SlotIdx + 1;
	}

	auto FirstHeapSlot = HeapSlot;
	auto Device = GetParentDevice()->GetDevice();
	CD3DX12_CPU_DESCRIPTOR_HANDLE SrcDescriptor;
	D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor = ViewHeap->GetCPUSlotHandle(FirstHeapSlot);
	const uint64 DescriptorSize = ViewHeap->GetDescriptorSize();

	for (uint32 SlotIdx = 0; SlotIdx < SlotsNeeded; SlotIdx++)
	{
		if (SRVs[SlotIdx])
		{
			SrcDescriptor = SRVs[SlotIdx]->GetView();
			Device->CopyDescriptorsSimple(1, DestDescriptor, SrcDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}
		DestDescriptor.ptr += DescriptorSize;
		HeapSlot++;
	}

	const D3D12_GPU_DESCRIPTOR_HANDLE BindDescriptor = ViewHeap->GetGPUSlotHandle(FirstHeapSlot);

	if (ShaderFrequency == SF_Compute)
	{
		const uint32 RDTIndex = RootSignature->SRVTableSlot(ShaderFrequency);
		CommandList->SetComputeRootDescriptorTable(RDTIndex, BindDescriptor);
	}
	else
	{
		const uint32 RDTIndex = RootSignature->SRVTableSlot(ShaderFrequency);
		CommandList->SetGraphicsRootDescriptorTable(RDTIndex, BindDescriptor);
	}

}

template<EShaderFrequency ShaderFrequency>
void FD3D12DescriptorCache::SetConstantBufferViews(ID3D12GraphicsCommandList* CommandList, FD3D12RootSignature* RootSignature, uint32 NumSlots, FD3D12ConstantBufferView* CBVs[], uint32& HeapSlot)
{

	uint32 SlotsNeeded = 0;
	for (uint32 SlotIdx = 0; SlotIdx < NumSlots; SlotIdx++)
	{
		if (CBVs[SlotIdx])
			SlotsNeeded = SlotIdx + 1;
	}

	auto FirstHeapSlot = HeapSlot;
	auto Device = GetParentDevice()->GetDevice();
	CD3DX12_CPU_DESCRIPTOR_HANDLE SrcDescriptor;
	D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor = ViewHeap->GetCPUSlotHandle(FirstHeapSlot);
	const uint64 DescriptorSize = ViewHeap->GetDescriptorSize();

	for (uint32 SlotIdx = 0; SlotIdx < SlotsNeeded; SlotIdx++)
	{
		if (CBVs[SlotIdx])
		{
			SrcDescriptor = CBVs[SlotIdx]->GetView();
			Device->CopyDescriptorsSimple(1, DestDescriptor, SrcDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}
		DestDescriptor.ptr += DescriptorSize;
		HeapSlot++;
	}

	const D3D12_GPU_DESCRIPTOR_HANDLE BindDescriptor = ViewHeap->GetGPUSlotHandle(FirstHeapSlot);

	if (ShaderFrequency == SF_Compute)
	{
		const uint32 RDTIndex = RootSignature->CBVTableSlot(ShaderFrequency);
		CommandList->SetComputeRootDescriptorTable(RDTIndex, BindDescriptor);
	}
	else
	{
		const uint32 RDTIndex = RootSignature->CBVTableSlot(ShaderFrequency);
		CommandList->SetGraphicsRootDescriptorTable(RDTIndex, BindDescriptor);
	}

}

template<EShaderFrequency ShaderFrequency>
void FD3D12DescriptorCache::SetUnorderedAccessViews(ID3D12GraphicsCommandList* CommandList, FD3D12RootSignature* RootSignature, uint32 NumSlots, FD3D12UnorderedAccessView* UAVs[], uint32& HeapSlot)
{

	uint32 SlotsNeeded = 0;
	for (uint32 SlotIdx = 0; SlotIdx < NumSlots; SlotIdx++)
	{
		if (UAVs[SlotIdx])
			SlotsNeeded = SlotIdx + 1;
	}

	auto FirstHeapSlot = HeapSlot;
	auto Device = GetParentDevice()->GetDevice();
	CD3DX12_CPU_DESCRIPTOR_HANDLE SrcDescriptor;
	D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor = ViewHeap->GetCPUSlotHandle(FirstHeapSlot);
	const uint64 DescriptorSize = ViewHeap->GetDescriptorSize();

	for (uint32 SlotIdx = 0; SlotIdx < SlotsNeeded; SlotIdx++)
	{
		if (UAVs[SlotIdx])
		{
			SrcDescriptor = UAVs[SlotIdx]->GetView();
			Device->CopyDescriptorsSimple(1, DestDescriptor, SrcDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}
		DestDescriptor.ptr += DescriptorSize;
		HeapSlot++;
	}

	const D3D12_GPU_DESCRIPTOR_HANDLE BindDescriptor = ViewHeap->GetGPUSlotHandle(FirstHeapSlot);

	if (ShaderFrequency == SF_Compute)
	{
		const uint32 RDTIndex = RootSignature->UAVTableSlot(ShaderFrequency);
		CommandList->SetComputeRootDescriptorTable(RDTIndex, BindDescriptor);
	}
	else
	{
		const uint32 RDTIndex = RootSignature->UAVTableSlot(ShaderFrequency);
		CommandList->SetGraphicsRootDescriptorTable(RDTIndex, BindDescriptor);
	}

}

template<EShaderFrequency ShaderFrequency>
void FD3D12DescriptorCache::SetSamplers(ID3D12GraphicsCommandList* CommandList, FD3D12RootSignature* RootSignature, uint32 NumSlots, FD3D12SamplerState* Samplers[])
{
	auto Device = GetParentDevice()->GetDevice();
	CD3DX12_CPU_DESCRIPTOR_HANDLE SrcDescriptor;
	D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor = SamplerHeap->GetCPUSlotHandle(0);
	const uint64 DescriptorSize = SamplerHeap->GetDescriptorSize();

	for (uint32 SlotIdx = 0; SlotIdx < NumSlots; SlotIdx++)
	{
		SrcDescriptor = Samplers[SlotIdx]->Descriptor;
		Device->CopyDescriptorsSimple(1, DestDescriptor, SrcDescriptor, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		DestDescriptor.ptr += DescriptorSize;
	}

	const D3D12_GPU_DESCRIPTOR_HANDLE BindDescriptor = SamplerHeap->GetGPUSlotHandle(0);

	if (ShaderFrequency == SF_Compute)
	{
		const uint32 RDTIndex = RootSignature->SamplerTableSlot(ShaderFrequency);
		CommandList->SetComputeRootDescriptorTable(RDTIndex, BindDescriptor);
	}
	else
	{
		const uint32 RDTIndex = RootSignature->SamplerTableSlot(ShaderFrequency);
		CommandList->SetGraphicsRootDescriptorTable(RDTIndex, BindDescriptor);
	}
}


void FD3D12DescriptorCache::InitFrame(ID3D12GraphicsCommandList* CommandList)
{

	ID3D12DescriptorHeap* descriptorHeaps[] = {
		ViewHeap->GetHeap(),
		SamplerHeap->GetHeap()
	};
	CommandList->SetDescriptorHeaps(2, descriptorHeaps);
}


void FD3D12FrameCache::ApplySRVs(ID3D12GraphicsCommandList* CommandList)
{

}

void FD3D12FrameCache::ApplyUAVs(ID3D12GraphicsCommandList* CommandList)
{

}

void FD3D12FrameCache::ApplyCBVs(ID3D12GraphicsCommandList* CommandList)
{

}

void FD3D12FrameCache::ApplyState(ID3D12GraphicsCommandList* CommandList)
{

	CommandList->SetPipelineState(BoundPSO->PipelineStateObject.Get());
	CommandList->SetGraphicsRootSignature(BoundPSO->RootSignature->RootSignature);

	// SRVS

#define COND_SET_SRVS(shader) \
	if (DirtySRVMask[shader]) \
	{ \
		DescriptorCache.SetShaderResourceViews<shader>(CommandList, BoundPSO->RootSignature, 16, BoundSRVs[shader], FirstHeapSlot); \
	} \


	COND_SET_SRVS(SF_Vertex);
	COND_SET_SRVS(SF_Pixel);
	COND_SET_SRVS(SF_Hull);
	COND_SET_SRVS(SF_Domain);
	COND_SET_SRVS(SF_Geometry);
	COND_SET_SRVS(SF_Compute);

	// CBVS

#define COND_SET_CBVS(shader) \
	if (DirtyCBVMask[shader]) \
	{ \
		DescriptorCache.SetConstantBufferViews<shader>(CommandList, BoundPSO->RootSignature, 16, BoundCBVs[shader], FirstHeapSlot); \
	} \


	COND_SET_CBVS(SF_Vertex);
	COND_SET_CBVS(SF_Pixel);
	COND_SET_CBVS(SF_Hull);
	COND_SET_CBVS(SF_Domain);
	COND_SET_CBVS(SF_Geometry);
	COND_SET_CBVS(SF_Compute);


	// UAVs
	/*
	if (DirtyUAVMask[0])
	{ 
		DescriptorCache.SetUnorderedAccessViews<SF_Pixel>(CommandList, BoundPSO->RootSignature, 16, BoundUAVs[0], FirstHeapSlot);
	} 
	if (DirtyUAVMask[1])
	{
		DescriptorCache.SetUnorderedAccessViews<SF_Compute>(CommandList, BoundPSO->RootSignature, 16, BoundUAVs[1], FirstHeapSlot);
	}
	*/

#define COND_SET_SAMPLERS(shader) \
	if (DirtySamplerMask[shader]) \
	{ \
		DescriptorCache.SetSamplers<shader>(CommandList, BoundPSO->RootSignature, 16, BoundSamplers[shader]); \
	} \


	COND_SET_SAMPLERS(SF_Vertex);
	COND_SET_SAMPLERS(SF_Pixel);
	COND_SET_SAMPLERS(SF_Hull);
	COND_SET_SAMPLERS(SF_Domain);
	COND_SET_SAMPLERS(SF_Geometry);
	COND_SET_SAMPLERS(SF_Compute);


	for (uint32 ShaderFreq = 0; ShaderFreq < EShaderFrequency::SF_ShaderStages; ShaderFreq++)
	{
		DirtyCBVMask[ShaderFreq] = 0;
		DirtySRVMask[ShaderFreq] = 0;
		DirtySamplerMask[ShaderFreq] = 0;

		for (uint32 SRVSlot = 0; SRVSlot < 16; SRVSlot++)
		{
			BoundSRVs[ShaderFreq][SRVSlot] = 0;
		}
	}
}

void FD3D12FrameCache::ApplyComputeState(ID3D12GraphicsCommandList* CommandList)
{

	CommandList->SetPipelineState(BoundPSO->PipelineStateObject.Get());
	CommandList->SetComputeRootSignature(BoundPSO->RootSignature->RootSignature);

	// SRVS

#define COND_SET_SRVS(shader) \
	if (DirtySRVMask[shader]) \
	{ \
		DescriptorCache.SetShaderResourceViews<shader>(CommandList, BoundPSO->RootSignature, 16, BoundSRVs[shader], FirstHeapSlot); \
	} \

	COND_SET_SRVS(SF_Compute);

	// CBVS

#define COND_SET_CBVS(shader) \
	if (DirtyCBVMask[shader]) \
	{ \
		DescriptorCache.SetConstantBufferViews<shader>(CommandList, BoundPSO->RootSignature, 16, BoundCBVs[shader], FirstHeapSlot); \
	} \

	COND_SET_CBVS(SF_Compute);

	// UAVs

	if (DirtyUAVMask[0])
	{
		DescriptorCache.SetUnorderedAccessViews<SF_Pixel>(CommandList, BoundPSO->RootSignature, 16, BoundUAVs[0], FirstHeapSlot);
	}
	if (DirtyUAVMask[1])
	{
		DescriptorCache.SetUnorderedAccessViews<SF_Compute>(CommandList, BoundPSO->RootSignature, 16, BoundUAVs[1], FirstHeapSlot);
	}

#define COND_SET_SAMPLERS(shader) \
	if (DirtySamplerMask[shader]) \
	{ \
		DescriptorCache.SetSamplers<shader>(CommandList, BoundPSO->RootSignature, 16, BoundSamplers[shader]); \
	} \

	COND_SET_SAMPLERS(SF_Compute);

	for (uint32 ShaderFreq = 0; ShaderFreq < EShaderFrequency::SF_ShaderStages; ShaderFreq++)
	{
		DirtyCBVMask[ShaderFreq] = 0;
		DirtySRVMask[ShaderFreq] = 0;
		DirtySamplerMask[ShaderFreq] = 0;

		for (uint32 SRVSlot = 0; SRVSlot < 16; SRVSlot++)
		{
			BoundSRVs[ShaderFreq][SRVSlot] = 0;
		}
	}
}