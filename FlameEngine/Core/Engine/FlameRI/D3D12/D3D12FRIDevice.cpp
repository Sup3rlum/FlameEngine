#include "D3D12FRICommon.h"

FD3D12Device::FD3D12Device(ComPtr<ID3D12Device> Device, ComPtr<ID3D12CommandQueue> CommandQueue, ComPtr<IDXGISwapChain4>  SwapChain, uint32 BackBufferCount, D3D12FRIContext* context) :
	Device(Device),
	CommandQueue(CommandQueue),
	SwapChain(SwapChain),
	RTVDescriptorHeap(new FD3D12OfflineDescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 256)),
	DSVDescriptorHeap(new FD3D12OfflineDescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 256)),
	SRVDescriptorHeap(new FD3D12OfflineDescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 256)),
	CBVDescriptorHeap(new FD3D12OfflineDescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 256)),
	UAVDescriptorHeap(new FD3D12OfflineDescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 256)),
	SamplerDescriptorHeap(new FD3D12OfflineDescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 256)),
	BackBufferCount(BackBufferCount),
	Context(context)
{

	CreateCommandContext();
	auto commandContext = GetCommandContext(0);
}

void FD3D12Device::CreateCommandContext()
{
	auto NewContext = new D3D12FRICommandContext(this);
	ContextArray.Add(NewContext);
}