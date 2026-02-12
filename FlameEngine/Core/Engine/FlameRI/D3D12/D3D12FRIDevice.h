#pragma once


class D3D12FRICommandContext;
class FD3D12OfflineDescriptorHeap;
class FD3D12OnlineDescriptorHeap;
class D3D12FRIContext;

class FD3D12Device
{
public:

	FD3D12Device(ComPtr<ID3D12Device> D3DDevice, ComPtr<ID3D12CommandQueue> CommandQueue, ComPtr<IDXGISwapChain4> SwapChain, uint32 BackBufferCount, D3D12FRIContext* context);
	void CreateCommandContext();


	D3D12FRICommandContext* GetCommandContext(uint32 ThreadIndex)
	{
		return ContextArray[ThreadIndex];
	}

	ID3D12Device* GetDevice()
	{
		return Device.Get();
	}


	template <typename TViewDesc> FD3D12OfflineDescriptorHeap* GetViewDescriptorAllocator();
	template<> FD3D12OfflineDescriptorHeap* GetViewDescriptorAllocator<D3D12_SHADER_RESOURCE_VIEW_DESC>() { return SRVDescriptorHeap; }
	template<> FD3D12OfflineDescriptorHeap* GetViewDescriptorAllocator<D3D12_RENDER_TARGET_VIEW_DESC>() { return RTVDescriptorHeap; }
	template<> FD3D12OfflineDescriptorHeap* GetViewDescriptorAllocator<D3D12_DEPTH_STENCIL_VIEW_DESC>() { return DSVDescriptorHeap; }
	template<> FD3D12OfflineDescriptorHeap* GetViewDescriptorAllocator<D3D12_UNORDERED_ACCESS_VIEW_DESC>() { return UAVDescriptorHeap; }
	template<> FD3D12OfflineDescriptorHeap* GetViewDescriptorAllocator<D3D12_CONSTANT_BUFFER_VIEW_DESC>() { return CBVDescriptorHeap; }


	ComPtr<ID3D12Device> Device;
	ComPtr<IDXGISwapChain4> SwapChain;
	ComPtr<ID3D12CommandQueue> CommandQueue;
	FArray<D3D12FRICommandContext*> ContextArray;
	D3D12FRICommandContext* CopyCommandContext;
	D3D12FRICommandContext* ComputeCommandContext;
	D3D12FRIContext* Context;

	FD3D12OfflineDescriptorHeap* RTVDescriptorHeap;
	FD3D12OfflineDescriptorHeap* DSVDescriptorHeap;
	FD3D12OfflineDescriptorHeap* UAVDescriptorHeap;
	FD3D12OfflineDescriptorHeap* SRVDescriptorHeap;
	FD3D12OfflineDescriptorHeap* CBVDescriptorHeap;
	FD3D12OfflineDescriptorHeap* SamplerDescriptorHeap;
	uint32 BackBufferCount;

};

class FD3D12DeviceChild
{
	FD3D12Device* Parent;

public:

	FD3D12DeviceChild(FD3D12Device* InParent) : Parent(InParent)
	{}

	FD3D12Device* GetParentDevice()
	{
		return Parent;
	}
};

