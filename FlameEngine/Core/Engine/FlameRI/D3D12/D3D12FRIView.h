#pragma once


template<typename TDesc>
class TD3D12ViewDescriptorHandle : public FD3D12DeviceChild
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE Handle;
	uint32 Index;

	template <typename TDesc> struct TCreateViewMap;
	template<> struct TCreateViewMap<D3D12_SHADER_RESOURCE_VIEW_DESC> { static decltype(&ID3D12Device::CreateShaderResourceView)	GetCreate() { return &ID3D12Device::CreateShaderResourceView; } };
	template<> struct TCreateViewMap<D3D12_RENDER_TARGET_VIEW_DESC> { static decltype(&ID3D12Device::CreateRenderTargetView)		GetCreate() { return &ID3D12Device::CreateRenderTargetView; } };
	template<> struct TCreateViewMap<D3D12_DEPTH_STENCIL_VIEW_DESC> { static decltype(&ID3D12Device::CreateDepthStencilView)		GetCreate() { return &ID3D12Device::CreateDepthStencilView; } };
	template<> struct TCreateViewMap<D3D12_UNORDERED_ACCESS_VIEW_DESC> { static decltype(&ID3D12Device::CreateUnorderedAccessView)	GetCreate() { return &ID3D12Device::CreateUnorderedAccessView; } };
	template<> struct TCreateViewMap<D3D12_CONSTANT_BUFFER_VIEW_DESC> { static decltype(&ID3D12Device::CreateConstantBufferView)	GetCreate() { return &ID3D12Device::CreateConstantBufferView; } };

public:
	void CreateView(const TDesc& InDesc, ID3D12Resource* Resource)
	{
		// Get creation function
		(GetParentDevice()->GetDevice()->*TCreateViewMap<TDesc>::GetCreate()) (Resource, &InDesc, Handle);
	}

	void CreateViewBindless(const TDesc& InDesc)
	{
		// Get creation function
		(GetParentDevice()->GetDevice()->*TCreateViewMap<TDesc>::GetCreate()) (&InDesc, Handle);
	}

	void CreateViewWithCounter(const TDesc& InDesc, ID3D12Resource* Resource, ID3D12Resource* CounterResource)
	{
		// Get creation function
		(GetParentDevice()->GetDevice()->*TCreateViewMap<TDesc>::GetCreate()) (Resource, CounterResource, &InDesc, Handle);
	}


	inline const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetHandle() const { return Handle; }

	TD3D12ViewDescriptorHandle(FD3D12Device* InParent) : 
		FD3D12DeviceChild(InParent)
	{
		FD3D12Device* Device = GetParentDevice();
		FD3D12OfflineDescriptorHeap* DescriptorAllocator = Device->template GetViewDescriptorAllocator<TDesc>();
		Handle = DescriptorAllocator->AllocateHeapSlot(Index);
	}

};

template<typename TDesc>
class FD3D12View 
{

public:

	FD3D12View(FD3D12Device* InParent) :
		Descriptor(InParent),
		Desc(TDesc{})
	{}

	TDesc Desc;
	TD3D12ViewDescriptorHandle<TDesc> Descriptor;
	FD3D12Resource* ResourceHandle;

	inline FD3D12Device* GetParentDevice()			const { return Descriptor.GetParentDevice(); }
	inline CD3DX12_CPU_DESCRIPTOR_HANDLE GetView()	const { return Descriptor.GetHandle(); }

	void CreateView(const TDesc& InDesc, FD3D12Resource* Resource)
	{
		Desc = InDesc;
		Descriptor.CreateView(InDesc, Resource->Resource.Get());
		ResourceHandle = Resource;
	}

	void CreateViewBindless(const TDesc& InDesc)
	{
		Desc = InDesc;
		Descriptor.CreateViewBindless(InDesc);
		ResourceHandle = nullptr;
	}


	void CreateViewWithCounter(const TDesc& InDesc, FD3D12Resource* Resource, ID3D12Resource* CounterResource)
	{
		Desc = InDesc;
		Descriptor.CreateViewWithCounter(InDesc, Resource->Resource.Get(), CounterResource);
		ResourceHandle = Resource;

	}
};

class FD3D12ShaderResourceView : public FRIShaderResourceView, public FD3D12View<D3D12_SHADER_RESOURCE_VIEW_DESC>
{
public:
	FD3D12ShaderResourceView(FD3D12Device* InParent, D3D12_SHADER_RESOURCE_VIEW_DESC& InDesc, FD3D12Resource* InResource)
		: FD3D12View(InParent)
	{
		CreateView(InDesc, InResource);
	}

};

class FD3D12UnorderedAccessView : public FRIUnorderedAccessView, public FD3D12View<D3D12_UNORDERED_ACCESS_VIEW_DESC>
{
public:
	ComPtr<ID3D12Resource> CounterResource;
	FD3D12UnorderedAccessView(FD3D12Device* InParent, D3D12_UNORDERED_ACCESS_VIEW_DESC& InDesc, FD3D12Resource* InResource) : 
		FD3D12View(InParent),
		CounterResource(0)
	{
		CreateViewWithCounter(InDesc, InResource, CounterResource.Get());
	}

};

class FD3D12RenderTargetView : public FRIRenderTargetView, public FD3D12View<D3D12_RENDER_TARGET_VIEW_DESC>
{
public:
	FD3D12RenderTargetView(FD3D12Device* InParent, D3D12_RENDER_TARGET_VIEW_DESC& InDesc, FD3D12Resource* InResource)
		: FD3D12View(InParent)
	{
		CreateView(InDesc, InResource);
	}

	FRITexture2D* GetDepthBuffer()
	{
		return 0;
	}
};

class FD3D12DepthStencilView : public FRIDepthStencilView, public FD3D12View<D3D12_DEPTH_STENCIL_VIEW_DESC>
{
public:
	FD3D12DepthStencilView(FD3D12Device* InParent, const D3D12_DEPTH_STENCIL_VIEW_DESC& InDSVDesc, FD3D12Resource* InResource)
		: FD3D12View(InParent)
	{
		CreateView(InDSVDesc, InResource);
	}
};

class FD3D12ConstantBufferView : public FRIConstantBufferView, public FD3D12View<D3D12_CONSTANT_BUFFER_VIEW_DESC>
{
public:
	FD3D12ConstantBufferView(FD3D12Device* InParent, const D3D12_CONSTANT_BUFFER_VIEW_DESC& InDSVDesc)
		: FD3D12View(InParent)
	{
		CreateViewBindless(InDSVDesc);
	}
};