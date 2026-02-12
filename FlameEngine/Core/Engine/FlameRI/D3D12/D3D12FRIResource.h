#pragma once


#define NUM_CUBEMAP_LAYERS 6
#define ZeroMem(a) ZeroMemory(&a, sizeof(a))

inline void D3DConvertDescriptorToSubresource(
	uint32 NumRows,
	uint32 NumSlices,
	uint32 NumLayers,
	uint32 NumMips,
	_In_ FRICreationDescriptor descIn[],
	_Out_ D3D12_SUBRESOURCE_DATA** descOut)
{
	if (descIn == nullptr)
	{
		*descOut = nullptr;
		return;
	}
	if (descIn[0].DataArray == nullptr || descIn[0].ByteSize == 0)
	{
		*descOut = nullptr;
		return;
	}

	auto NumSubresources = NumLayers * NumMips;
	auto pOut = new D3D12_SUBRESOURCE_DATA[NumSubresources];
	ZeroMemory(pOut, sizeof(D3D12_SUBRESOURCE_DATA) * NumSubresources);

	for (auto layer : FRange(NumLayers))
	{
		for (auto mip : FRange(NumMips))
		{
			uint32 idx = D3D12CalcSubresource(mip, layer, 0, NumMips, 0);
			uint32 rowCount = NumRows / pow(2, mip);
			uint32 sliceCount = NumSlices / pow(2, mip);

			if (NumSlices > 1)
			{
				pOut[idx].pData = descIn[idx].DataArray;
				pOut[idx].RowPitch = descIn[idx].ByteSize / (rowCount * sliceCount);
				pOut[idx].SlicePitch = descIn[idx].ByteSize / rowCount;
			}
			else
			{
				pOut[idx].pData = descIn[idx].DataArray;
				pOut[idx].RowPitch = descIn[idx].ByteSize / rowCount;
				pOut[idx].SlicePitch = 0;
			}
		}
	}
	*descOut = pOut;
}

inline D3D12_SUBRESOURCE_DATA D3DConvertDescriptorToSubresourceBuffer(
	uint32 BufferSize,
	_In_ FRIUpdateDescriptor descIn)
{
	D3D12_SUBRESOURCE_DATA pOut;
	ZeroMemory(&pOut, sizeof(D3D12_SUBRESOURCE_DATA));

	pOut.pData = descIn.DataArray;
	pOut.RowPitch = descIn.RowPitch;
	pOut.SlicePitch = 0;

	return pOut;
}


struct FD3D12Resource : public FD3D12DeviceChild, public FRIResource
{
	ComPtr<ID3D12Resource> Resource;
	D3D12_RESOURCE_STATES CurrentState;

	FD3D12Resource(FD3D12Device* Parent) : FD3D12DeviceChild(Parent)
	{}

	FD3D12Resource(FString Name,
		FD3D12Device* Parent,
		D3D12_HEAP_PROPERTIES HeapProperties,
		D3D12_HEAP_FLAGS HeapFlags,
		D3D12_RESOURCE_DESC ResourceDesc,
		D3D12_RESOURCE_STATES InitialState,
		D3D12_CLEAR_VALUE* OptimizedClearValue
		) :
		FD3D12DeviceChild(Parent),
		CurrentState(InitialState)
	{

		auto ClearValue = ResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER ? nullptr : OptimizedClearValue;
		ThrowIfFailed(Parent->Device->CreateCommittedResource(
			&HeapProperties,
			HeapFlags,
			&ResourceDesc,
			InitialState,
			ClearValue,
			IID_PPV_ARGS(Resource.GetAddressOf())
		));
		Resource->SetName(Name.ToPlatformString());
	}

	void TransitionState(D3D12_RESOURCE_STATES fromState, D3D12_RESOURCE_STATES toState);
	void TransitionState(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES toState);


	/*FORCEINLINE void UpdateSubresources(uint32 NumSubresources, const D3D12_SUBRESOURCE_DATA* Data)
	{
	}*/

};

struct FD3D12TransientResource : public FD3D12DeviceChild
{

	FD3D12Resource* TargetResource;
	FD3D12Resource* IntermediateResource;
	D3D12_RESOURCE_STATES WorkingState;

	FD3D12TransientResource(FD3D12Device* Parent, D3D12_RESOURCE_STATES WorkingState) : 
		FD3D12DeviceChild(Parent),
		TargetResource(NULL),
		IntermediateResource(NULL),
		WorkingState(WorkingState)
	{

	}

	void UpdateSubresources(uint32 NumSubresources, const D3D12_SUBRESOURCE_DATA* Data);
	void UpdateSubresources(ID3D12GraphicsCommandList* CommandList, uint32 NumSubresources, const D3D12_SUBRESOURCE_DATA* Data);

	void RenameResources(const FString& Name);

};

/*
struct FD3D11BufferBase
{
	//ComPtr<ID3D11Buffer> Buffer;

	/*ID3D11Resource* D3DResource() override
	{
		return Buffer.Get();
	}
};


struct FD3D12VertexBuffer : public FRIVertexBuffer, FD3D11BufferBase
{
	//ComPtr<ID3D11InputLayout> InputLayout;
	//uint32 LayoutStride;


};

struct FD3D11IndexBuffer : FRIIndexBuffer, FD3D11BufferBase
{
	ComPtr<ID3D12Resource> IndexBuffer;
	D3D12_INDEX_BUFFER_VIEW View;

	FD3D11IndexBuffer(
		ComPtr<ID3D12Device> Device,
		uint32 IndexCount,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor Data) :
		FRIIndexBuffer(IndexCount, Usage, EFRIAccess::None)
	{

		ThrowIfFailed(Device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(IndexCount),
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&IndexBuffer)));

		NAME_D3D12_OBJECT(IndexBuffer);

		/* {
			ThrowIfFailed(Device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(Data.ByteSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_indexBufferUpload)));

			// Copy data to the upload heap and then schedule a copy 
			// from the upload heap to the index buffer.
			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = Data.DataArray;
			indexData.RowPitch = Data.ByteSize;
			indexData.SlicePitch = indexData.RowPitch;

			//PIXBeginEvent(commandList.Get(), 0, L"Copy index buffer data to default resource...");

			UpdateSubresources<1>(commandList.Get(), IndexBuffer.Get(), m_indexBufferUpload.Get(), 0, 0, 1, &indexData);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(IndexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

			//PIXEndEvent(commandList.Get());
		}

		// Initialize the index buffer view.
		View.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
		View.SizeInBytes = Data.ByteSize;
		View.Format = DXGI_FORMAT_R32_UINT;
	}

	FRIResource* GetResource() override
	{
		return NULL;
	}
};
/*

struct FD3D11InstanceBuffer : FRIInstanceBuffer, FD3D11BufferBase
{
	uint32 LayoutStride;

	FD3D11InstanceBuffer(
		ComPtr<ID3D12Device> device,
		uint32 InstanceCount,
		uint32 InstanceStride,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor Data) :
		FRIInstanceBuffer(InstanceCount, InstanceStride, Usage, Access),
		LayoutStride(InstanceStride)
	{
		D3D11_BUFFER_DESC BufferDesc;
		BufferDesc.ByteWidth = this->GetByteSize();
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3DGetAccess(Access);
		BufferDesc.Usage = D3DGetUsage(Usage);
		BufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA* BufferData;
		D3DConvertDescriptorToSubresource(
			1, 1, 1, &Data, &BufferData
		);
		D3D_HRESULT(device->CreateBuffer(&BufferDesc, BufferData, &Buffer));

	}

	FRIResource* GetResource() override
	{
		return this;
	}
};


struct FD3D11ConstantBuffer : FRIConstantBuffer, FD3D11BufferBase
{
	FD3D11ConstantBuffer(ComPtr<ID3D11Device> device,
		uint32 ByteSize,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor Data
	) :
		FRIConstantBuffer(ByteSize, 0, Usage, Access)
	{

		D3D11_BUFFER_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

		BufferDesc.ByteWidth = ByteSize;
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = D3DGetAccess(Access);
		BufferDesc.Usage = D3DGetUsage(Usage);
		BufferDesc.MiscFlags = 0;


		D3D11_SUBRESOURCE_DATA* BufferData;
		D3DConvertDescriptorToSubresource(
			1, 1, 1, &Data, &BufferData
		);
		D3D_HRESULT(device->CreateBuffer(&BufferDesc, BufferData, Buffer.GetAddressOf()));
	}

	FRIResource* GetResource() override
	{
		return this;
	}
};

struct FD3D11ComputeBuffer : FRIComputeBuffer, FD3D11BufferBase, FD3D11UnorderedAccessView
{
	FD3D11ComputeBuffer(
		ComPtr<ID3D11Device> device,
		uint32 StructureCount,
		uint32 StructureStride,
		EFRIAccess Access,
		EFRIUsage Usage,
		FRICreationDescriptor Data
	) :
		FRIComputeBuffer(StructureCount, StructureStride, Usage, Access),
		FD3D11UnorderedAccessView(0)
	{

		D3D11_BUFFER_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

		BufferDesc.ByteWidth = this->GetByteSize();
		BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		BufferDesc.CPUAccessFlags = D3DGetAccess(Access);
		BufferDesc.Usage = D3DGetUsage(Usage);
		BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		BufferDesc.StructureByteStride = StructureStride;


		D3D11_SUBRESOURCE_DATA* BufferData;
		D3DConvertDescriptorToSubresource(
			1, 1, 1, &Data, &BufferData
		);
		D3D_HRESULT(device->CreateBuffer(&BufferDesc, BufferData, Buffer.GetAddressOf()));
	}

	void CreateUAV(ComPtr<ID3D11Device> device) override
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
		ZeroMemory(&UAVDesc, sizeof(UAVDesc));
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.NumElements = StructureCount;

		D3D_HRESULT(device->CreateUnorderedAccessView(Buffer.Get(), &UAVDesc, UnorderedAccessView.GetAddressOf()));
	}

	FRIResource* GetResource() override
	{
		return this;
	}
	FRIUnorderedAccessView* ComputeView() override
	{
		return this;
	}
};


struct FD3D11TextureBase : FD3DResource
{
	ComPtr<ID3D11Texture2D> Texture;
	DXGI_FORMAT Format;

	FD3D11TextureBase(DXGI_FORMAT format) :
		Format(format)
	{}

	virtual ~FD3D11TextureBase()
	{}

	ID3D11Resource* D3DResource() override
	{
		return Texture.Get();
	}
};


struct FD3D11TextureBase3D : FD3DResource
{
	ComPtr<ID3D11Texture3D> Texture;
	DXGI_FORMAT Format;

	FD3D11TextureBase3D(DXGI_FORMAT format) :
		Format(format)
	{}

	virtual ~FD3D11TextureBase3D()
	{}

	ID3D11Resource* D3DResource() override
	{
		return Texture.Get();
	}
};


struct FD3D11Texture2D : FD3D11TextureBase, FRITexture2D, FD3D11ShaderResourceView, FD3D11UnorderedAccessView
{
	FD3D11Texture2D(
		ComPtr<ID3D11Device> device,
		uint32 Width,
		uint32 Height,
		uint32 MipLevels,
		DXGI_FORMAT format,
		D3D11_USAGE Usage,
		UINT Access,
		UINT BindFlags,
		UINT MiscFlags,
		FRICreationDescriptor Data[]
	) :
		FRITexture2D(Width, Height, MipLevels),
		FD3D11ShaderResourceView(0),
		FD3D11UnorderedAccessView(0),
		FD3D11TextureBase(format)
	{

		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMem(TextureDesc);

		TextureDesc.Width = Width;
		TextureDesc.Height = Height;
		TextureDesc.MipLevels = MipLevels;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = format;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = Usage;
		TextureDesc.BindFlags = BindFlags;
		TextureDesc.CPUAccessFlags = Access;
		TextureDesc.MiscFlags = MiscFlags;


		auto subResources = MipLevels == -1 ? sizeof(Data) / sizeof(FRICreationDescriptor) : MipLevels;

		D3D11_SUBRESOURCE_DATA* TextureData;
		D3DConvertDescriptorToSubresource(
			1, subResources, Height, Data, &TextureData
		);
		D3D_HRESULT(device->CreateTexture2D(&TextureDesc, TextureData, Texture.GetAddressOf()));

	}

	void CreateSRV(ComPtr<ID3D11Device> device, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN) override
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;

		SrvDesc.Format = format == DXGI_FORMAT_UNKNOWN ? Format : format;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SrvDesc.Texture2D.MostDetailedMip = 0;
		SrvDesc.Texture2D.MipLevels = -1;

		/*
		if (bindDepth)
		{
			SrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}

		D3D_HRESULT(device->CreateShaderResourceView(Texture.Get(), &SrvDesc, ShaderResourceView.GetAddressOf()));
	}

	void CreateUAV(ComPtr<ID3D11Device> device)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UavDesc;
		ZeroMem(UavDesc);
		UavDesc.Format = DXGI_FORMAT_UNKNOWN;
		UavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		UavDesc.Texture2D.MipSlice = 0;

		D3D_HRESULT(device->CreateUnorderedAccessView(Texture.Get(), &UavDesc, UnorderedAccessView.GetAddressOf()));
	}

	FRIResource* GetResource() override
	{
		return this;
	}

	FRIShaderResourceView* View() override
	{
		return this;
	}
	FRIUnorderedAccessView* ComputeView() override
	{
		return this;
	}

};



struct FD3D11Texture2DArray : FD3D11TextureBase, FD3D11ShaderResourceView, FD3D11UnorderedAccessView, FRITexture2DArray
{
	FD3D11Texture2DArray(
		ComPtr<ID3D11Device> device,
		uint32 Width,
		uint32 Height,
		uint32 NumLayers,
		uint32 MipLevels,
		DXGI_FORMAT format,
		D3D11_USAGE Usage,
		UINT Access,
		UINT BindFlags,
		UINT MiscFlags,
		FRICreationDescriptor Data[]
	) :
		FRITexture2DArray(Width, Height, NumLayers, MipLevels),
		FD3D11ShaderResourceView(0),
		FD3D11UnorderedAccessView(0),
		FD3D11TextureBase(format)
	{

		D3D11_TEXTURE2D_DESC TextureDesc;
		TextureDesc.Width = Width;
		TextureDesc.Height = Height;
		TextureDesc.MipLevels = MipLevels;
		TextureDesc.ArraySize = NumLayers;
		TextureDesc.Format = format;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = Usage;
		TextureDesc.BindFlags = BindFlags;
		TextureDesc.CPUAccessFlags = Access;
		TextureDesc.MiscFlags = MiscFlags;


		D3D11_SUBRESOURCE_DATA* TextureData;
		D3DConvertDescriptorToSubresource(
			NumLayers, MipLevels, Height, Data, &TextureData
		);
		D3D_HRESULT(device->CreateTexture2D(&TextureDesc, TextureData, Texture.GetAddressOf()));
	}

	void CreateSRV(ComPtr<ID3D11Device> device, DXGI_FORMAT srvFormat = DXGI_FORMAT_UNKNOWN) override
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;
		ZeroMem(SrvDesc);
		SrvDesc.Format = srvFormat == DXGI_FORMAT_UNKNOWN ? Format : srvFormat;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		SrvDesc.Texture2DArray.MostDetailedMip = 0;
		SrvDesc.Texture2DArray.ArraySize = NumLayers;
		SrvDesc.Texture2DArray.FirstArraySlice = 0;
		SrvDesc.Texture2DArray.MipLevels = -1;

		D3D_HRESULT(device->CreateShaderResourceView(Texture.Get(), &SrvDesc, ShaderResourceView.GetAddressOf()));
	}

	void CreateUAV(ComPtr<ID3D11Device> device)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UavDesc;
		ZeroMem(UavDesc);
		UavDesc.Format = DXGI_FORMAT_UNKNOWN;
		UavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		UavDesc.Texture2DArray.ArraySize = NumLayers;
		UavDesc.Texture2DArray.FirstArraySlice = 0;
		UavDesc.Texture2DArray.MipSlice = 0;

		D3D_HRESULT(device->CreateUnorderedAccessView(Texture.Get(), &UavDesc, UnorderedAccessView.GetAddressOf()));
	}

	FRIResource* GetResource() override
	{
		return this;
	}

	FRIShaderResourceView* View() override
	{
		return this;
	}
	FRIUnorderedAccessView* ComputeView() override
	{
		return this;
	}
};


struct FD3D11Texture3D : FD3D11TextureBase3D, FD3D11ShaderResourceView, FD3D11UnorderedAccessView, FRITexture3D
{
	FD3D11Texture3D(ComPtr<ID3D11Device> device,
		uint32 Width,
		uint32 Height,
		uint32 Depth,
		uint32 MipLevels,
		DXGI_FORMAT format,
		D3D11_USAGE Usage,
		UINT Access,
		UINT BindFlags,
		UINT MiscFlags,
		FRICreationDescriptor Data[]
	) :
		FRITexture3D(Width, Height, Depth, MipLevels),
		FD3D11ShaderResourceView(0),
		FD3D11UnorderedAccessView(0),
		FD3D11TextureBase3D(format)
	{

		D3D11_TEXTURE3D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(TextureDesc));
		TextureDesc.Width = Width;
		TextureDesc.Height = Height;
		TextureDesc.Depth = Depth;
		TextureDesc.MipLevels = MipLevels;
		TextureDesc.Format = format;
		TextureDesc.Usage = Usage;
		TextureDesc.BindFlags = BindFlags;
		TextureDesc.CPUAccessFlags = Access;
		TextureDesc.MiscFlags = MiscFlags;


		D3D11_SUBRESOURCE_DATA* TextureData;
		D3DConvertDescriptorToSubresource(
			1, MipLevels, Height, Data, &TextureData
		);
		D3D_HRESULT(device->CreateTexture3D(&TextureDesc, TextureData, Texture.GetAddressOf()));

	}
	void CreateSRV(ComPtr<ID3D11Device> device, DXGI_FORMAT srvFormat = DXGI_FORMAT_UNKNOWN) override
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;
		ZeroMem(SrvDesc);
		SrvDesc.Format = srvFormat == DXGI_FORMAT_UNKNOWN ? Format : srvFormat;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		SrvDesc.Texture3D.MostDetailedMip = 0;
		SrvDesc.Texture3D.MipLevels = -1;


		D3D_HRESULT(device->CreateShaderResourceView(Texture.Get(), &SrvDesc, ShaderResourceView.GetAddressOf()));
	}

	void CreateUAV(ComPtr<ID3D11Device> device)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UavDesc;
		ZeroMem(UavDesc);
		UavDesc.Format = DXGI_FORMAT_UNKNOWN;
		UavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
		UavDesc.Texture3D.MipSlice = 0;
		UavDesc.Texture3D.WSize = Depth;

		D3D_HRESULT(device->CreateUnorderedAccessView(Texture.Get(), &UavDesc, UnorderedAccessView.GetAddressOf()));
	}


	FRIResource* GetResource() override
	{
		return this;
	}

	FRIShaderResourceView* View() override
	{
		return this;
	}
	FRIUnorderedAccessView* ComputeView() override
	{
		return this;
	}
};

struct FD3D11TextureCubeMap : FD3D11TextureBase, FD3D11ShaderResourceView, FD3D11UnorderedAccessView, FRITextureCubeMap
{
	FD3D11TextureCubeMap(ComPtr<ID3D11Device> device,
		uint32 Width,
		uint32 Height,
		uint32 MipLevels,
		DXGI_FORMAT format,
		D3D11_USAGE Usage,
		UINT Access,
		UINT BindFlags,
		UINT MiscFlags,
		FRICreationDescriptor Data[]
	) :
		FRITextureCubeMap(Width, Height, MipLevels),
		FD3D11ShaderResourceView(0),
		FD3D11UnorderedAccessView(0),
		FD3D11TextureBase(format)
	{

		D3D11_TEXTURE2D_DESC TextureDesc;
		TextureDesc.Width = Width;
		TextureDesc.Height = Height;
		TextureDesc.MipLevels = MipLevels;
		TextureDesc.ArraySize = NUM_CUBEMAP_LAYERS;
		TextureDesc.Format = format;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = Usage;
		TextureDesc.BindFlags = BindFlags;
		TextureDesc.CPUAccessFlags = Access;
		TextureDesc.MiscFlags = MiscFlags;


		D3D11_SUBRESOURCE_DATA* TextureData;
		D3DConvertDescriptorToSubresource(
			NUM_CUBEMAP_LAYERS, MipLevels, Height, Data, &TextureData
		);
		D3D_HRESULT(device->CreateTexture2D(&TextureDesc, TextureData, Texture.GetAddressOf()));
	}

	void CreateSRV(ComPtr<ID3D11Device> device, DXGI_FORMAT srvFormat = DXGI_FORMAT_UNKNOWN) override
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;
		ZeroMem(SrvDesc);
		SrvDesc.Format = srvFormat == DXGI_FORMAT_UNKNOWN ? Format : srvFormat;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		SrvDesc.TextureCube.MostDetailedMip = 0;
		SrvDesc.TextureCube.MipLevels = -1;


		D3D_HRESULT(device->CreateShaderResourceView(Texture.Get(), &SrvDesc, ShaderResourceView.GetAddressOf()));
	}

	void CreateUAV(ComPtr<ID3D11Device> device)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UavDesc;
		ZeroMem(UavDesc);
		UavDesc.Format = DXGI_FORMAT_UNKNOWN;
		UavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		UavDesc.Texture2DArray.ArraySize = NUM_CUBEMAP_LAYERS;
		UavDesc.Texture2DArray.FirstArraySlice = 0;
		UavDesc.Texture2DArray.MipSlice = 0;

		D3D_HRESULT(device->CreateUnorderedAccessView(Texture.Get(), &UavDesc, UnorderedAccessView.GetAddressOf()));
	}

	FRIResource* GetResource() override
	{
		return this;
	}
	FRIShaderResourceView* View() override
	{
		return this;
	}
	FRIUnorderedAccessView* ComputeView() override
	{
		return this;
	}
};

struct FD3D11SamplerState : FRISamplerState
{
	ComPtr<ID3D11SamplerState> Sampler;

	FD3D11SamplerState(
		ComPtr<ID3D11Device> device,
		EFRITextureFilter Filter,
		EFRITextureAddress AddressU,
		EFRITextureAddress AddressV,
		EFRITextureAddress AddressW,
		Color32 BorderColor = Color32())
	{

		float MaxAniso = 4.0f;

		if (Filter == EFRITextureFilter::Anisotropic4) MaxAniso = 4.0f;
		if (Filter == EFRITextureFilter::Anisotropic8) MaxAniso = 8.0f;

		D3D11_SAMPLER_DESC SamplerDesc;

		SamplerDesc.Filter = D3DGetTextureFilter(Filter);
		SamplerDesc.AddressU = D3DGetTextureAddressMode(AddressU);
		SamplerDesc.AddressV = D3DGetTextureAddressMode(AddressV);
		SamplerDesc.AddressW = D3DGetTextureAddressMode(AddressW);
		SamplerDesc.MaxAnisotropy = MaxAniso;
		SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamplerDesc.BorderColor[0] = BorderColor.r;
		SamplerDesc.BorderColor[1] = BorderColor.g;
		SamplerDesc.BorderColor[2] = BorderColor.b;
		SamplerDesc.BorderColor[3] = BorderColor.a;
		SamplerDesc.MinLOD = 0;
		SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		SamplerDesc.MipLODBias = 0;

		D3D_HRESULT(device->CreateSamplerState(&SamplerDesc, Sampler.GetAddressOf()));
	}
};


template<typename TShader>
struct TD3D11ShaderBase
{
	ComPtr<TShader> Shader;

	virtual ~TD3D11ShaderBase()
	{
	}
};


struct FD3D11VertexShader : FRIVertexShader, TD3D11ShaderBase<ID3D11VertexShader>
{
	FArray<byte> ByteCode;

	FD3D11VertexShader(ComPtr<ID3D11Device> device, const FArray<byte>& binCode) :
		ByteCode(binCode)
	{
		device->CreateVertexShader(binCode.Begin(), binCode.ByteSize(), NULL, Shader.GetAddressOf());
	}
};

struct FD3D11PixelShader : FRIPixelShader, TD3D11ShaderBase<ID3D11PixelShader>
{
	FD3D11PixelShader(ComPtr<ID3D11Device> device, const FArray<byte>& binCode)
	{
		device->CreatePixelShader(binCode.Begin(), binCode.ByteSize(), NULL, Shader.GetAddressOf());
	}
};

struct FD3D11GeometryShader : FRIGeometryShader, TD3D11ShaderBase<ID3D11GeometryShader>
{

	FD3D11GeometryShader(ComPtr<ID3D11Device> device, const FArray<byte>& binCode)
	{
		device->CreateGeometryShader(binCode.Begin(), binCode.ByteSize(), NULL, Shader.GetAddressOf());
	}
};

struct FD3D11HullShader : FRIHullShader, TD3D11ShaderBase<ID3D11HullShader>
{

	FD3D11HullShader(ComPtr<ID3D11Device> device, const FArray<byte>& binCode)
	{
		device->CreateHullShader(binCode.Begin(), binCode.ByteSize(), NULL, Shader.GetAddressOf());
	}
};

struct FD3D11DomainShader : FRIDomainShader, TD3D11ShaderBase<ID3D11DomainShader>
{

	FD3D11DomainShader(ComPtr<ID3D11Device> device, const FArray<byte>& binCode)
	{
		device->CreateDomainShader(binCode.Begin(), binCode.ByteSize(), NULL, Shader.GetAddressOf());
	}
};

struct FD3D11ComputeShader : FRIComputeShader, TD3D11ShaderBase<ID3D11ComputeShader>
{
	FD3D11ComputeShader(ComPtr<ID3D11Device> device, const FArray<byte>& binCode)
	{
		device->CreateComputeShader(binCode.Begin(), binCode.ByteSize(), NULL, Shader.GetAddressOf());
	}
};


struct FD3D11ShaderPipeline : FRIShaderPipeline
{
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;
	ID3D11DomainShader* domainShader;
	ID3D11HullShader* hullShader;
	ID3D11ComputeShader* computeShader;


	FD3D11ShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor, const FArray<EFRIShaderType>& types) :
		FRIShaderPipeline(descriptor),
		vertexShader(0),
		pixelShader(0),
		geometryShader(0),
		hullShader(0),
		domainShader(0),
		computeShader(0)
	{
		for (int i = 0; i < types.Length(); i++)
		{
			switch (types[i])
			{
			case EFRIShaderType::Vertex:	vertexShader = static_cast<FD3D11VertexShader*>(descriptor.ShaderArray[i])->Shader.Get(); break;
			case EFRIShaderType::Hull:		hullShader = static_cast<FD3D11HullShader*>(descriptor.ShaderArray[i])->Shader.Get(); break;
			case EFRIShaderType::Domain:	domainShader = static_cast<FD3D11DomainShader*>(descriptor.ShaderArray[i])->Shader.Get(); break;
			case EFRIShaderType::Geometry:	geometryShader = static_cast<FD3D11GeometryShader*>(descriptor.ShaderArray[i])->Shader.Get(); break;
			case EFRIShaderType::Pixel:		pixelShader = static_cast<FD3D11PixelShader*>(descriptor.ShaderArray[i])->Shader.Get(); break;
			case EFRIShaderType::Compute:	computeShader = static_cast<FD3D11ComputeShader*>(descriptor.ShaderArray[i])->Shader.Get(); break;
			}
		}
	}

	FRIResource* GetResource() override
	{
		return NULL;
	}
};
*/

/*
struct FD3D11InputLayout : FRIInputLayout
{
	ComPtr<ID3D11InputLayout> InputLayout;
	FArray<uint32> LayoutStrides;

	FD3D11InputLayout(

		ComPtr<ID3D11Device> device,
		const FArray<FRIInputDesc>& DescArray,
		FD3D11VertexShader* shaderSignature
	) :
		FRIInputLayout(DescArray)
	{
		uint32 NumLayouts = DescArray.Length();
		uint32 NumLayoutElems = 0;

		for (int i = 0; i < NumLayouts; i++)
		{
			NumLayoutElems += DescArray[i].Components.Length();
			LayoutStrides.Add(DescArray[i].Components[0].Stride);
		}

		FArray<D3D11_INPUT_ELEMENT_DESC> vertexLayout;


		for (int layoutIndex = 0; layoutIndex < NumLayouts; layoutIndex++)
		{

			for (int i = 0; i < DescArray[layoutIndex].Components.Length(); i++)
			{
				const FRIInputAttribute& Component = DescArray[layoutIndex].Components[i];

				D3D11_INPUT_ELEMENT_DESC ElemDesc;
				ZeroMem(ElemDesc);

				DXGI_FORMAT format;

				switch (Component.Type)
				{
				case EFRIAttributeType::Float:
					format = DXGI_FORMAT_R32_FLOAT;
					break;
				case EFRIAttributeType::Float2:
					format = DXGI_FORMAT_R32G32_FLOAT;
					break;
				case EFRIAttributeType::Float3:
					format = DXGI_FORMAT_R32G32B32_FLOAT;
					break;
				case EFRIAttributeType::Float4:
					format = DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
				case EFRIAttributeType::Int:
					format = DXGI_FORMAT_R32_SINT;
					break;
				case EFRIAttributeType::Int2:
					format = DXGI_FORMAT_R32G32_SINT;
					break;
				case EFRIAttributeType::Int3:
					format = DXGI_FORMAT_R32G32B32_SINT;
					break;
				case EFRIAttributeType::Int4:
					format = DXGI_FORMAT_R32G32B32A32_SINT;
					break;
				default:
					format = DXGI_FORMAT_UNKNOWN;
					break;
				}

				ElemDesc.SemanticName = Component.Semantic.SemanticName;
				ElemDesc.SemanticIndex = Component.Semantic.SemanticIndex;
				ElemDesc.Format = format;
				ElemDesc.InputSlot = DescArray[layoutIndex].InputSlot;
				ElemDesc.AlignedByteOffset = Component.Offset;

				if (Component.Usage == EFRIAttribUsage::PerVertex)
				{
					ElemDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
					ElemDesc.InstanceDataStepRate = 0;
				}
				else
				{
					ElemDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
					ElemDesc.InstanceDataStepRate = 1;
				}

				vertexLayout.Add(ElemDesc);
			}
		}

		D3D_HRESULT(device->CreateInputLayout(vertexLayout.Begin(), NumLayoutElems, shaderSignature->ByteCode.Begin(), shaderSignature->ByteCode.ByteSize(), InputLayout.GetAddressOf()));
	}
};


struct FD3D11RasterizerState : FRIRasterizerState
{
	ComPtr<ID3D11RasterizerState> RasterizerState;

	FD3D11RasterizerState(ComPtr<ID3D11Device> device,
		D3D11_CULL_MODE cullMode,
		D3D11_FILL_MODE fillMode
	)
	{
		D3D11_RASTERIZER_DESC RasterizerDesc;
		ZeroMem(RasterizerDesc);

		RasterizerDesc.AntialiasedLineEnable = false;
		RasterizerDesc.CullMode = cullMode;
		RasterizerDesc.FillMode = fillMode;
		RasterizerDesc.DepthBias = 0;
		RasterizerDesc.DepthBiasClamp = 0;
		RasterizerDesc.DepthClipEnable = TRUE;
		RasterizerDesc.FrontCounterClockwise = FALSE;
		RasterizerDesc.MultisampleEnable = FALSE;
		RasterizerDesc.ScissorEnable = FALSE;
		RasterizerDesc.SlopeScaledDepthBias = 0.0f;

		D3D_HRESULT(device->CreateRasterizerState(&RasterizerDesc, RasterizerState.GetAddressOf()));
	}
};


struct FD3D11BlendState : FRIBlendState
{
	ComPtr<ID3D11BlendState> BlendState;

	FD3D11BlendState(
		ComPtr<ID3D11Device> device,

		D3D11_BLEND SrcBlend,
		D3D11_BLEND DstBlend,
		D3D11_BLEND SrcAlphaBlend,
		D3D11_BLEND DstAlphaBlend
	)
	{
		D3D11_BLEND_DESC BlendDesc;
		ZeroMem(BlendDesc);

		BlendDesc.RenderTarget[0].BlendEnable = TRUE;
		BlendDesc.RenderTarget[0].SrcBlend = SrcBlend;
		BlendDesc.RenderTarget[0].DestBlend = DstBlend;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = SrcAlphaBlend;
		BlendDesc.RenderTarget[0].DestBlendAlpha = DstAlphaBlend;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D_HRESULT(device->CreateBlendState(&BlendDesc, BlendState.GetAddressOf()));
	}
};


struct FD3D11DepthStencilState : FRIDepthStencilState
{
	ComPtr<ID3D11DepthStencilState> DepthStencilState;

	FD3D11DepthStencilState(
		ComPtr<ID3D11Device> device,
		EFRIBool DepthEnable,
		EFRIBool StencilEnable

	)
	{
		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
		ZeroMem(DepthStencilDesc);

		DepthStencilDesc.DepthEnable = (BOOL)DepthEnable;
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		DepthStencilDesc.StencilEnable = (BOOL)StencilEnable;

		D3D_HRESULT(device->CreateDepthStencilState(&DepthStencilDesc, DepthStencilState.GetAddressOf()));

	}
};*/