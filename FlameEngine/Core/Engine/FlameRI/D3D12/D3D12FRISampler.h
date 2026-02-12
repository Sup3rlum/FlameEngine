#pragma once



class FD3D12SamplerState : public FRISamplerState, public FD3D12DeviceChild
{
public:
	D3D12_CPU_DESCRIPTOR_HANDLE Descriptor;
	uint32 DescriptorHeapIndex;

	FD3D12SamplerState(FD3D12Device* Parent, const D3D12_SAMPLER_DESC& Desc);

};