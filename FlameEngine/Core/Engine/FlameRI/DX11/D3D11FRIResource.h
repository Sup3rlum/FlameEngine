#pragma once


#include "../FRIResource.h"
#include "D3D11.h"
#include "D3D11FRIProxyTypes.h"



struct FD3D11ShaderResource
{
	TComPtr<ID3D11ShaderResourceView> ShaderResourceView;
	FD3D11ShaderResource(ID3D11ShaderResourceView* srv) : ShaderResourceView(srv)
	{}

	virtual ~FD3D11ShaderResource()
	{}
};

struct FD3D11VertexBuffer : FRIVertexBuffer
{
	TComPtr<ID3D11Buffer> Buffer;
	TComPtr<ID3D11InputLayout> InputLayout;

	uint32 LayoutStride;

	FD3D11VertexBuffer(ID3D11Device* device, uint32 Size, uint32 Usage, EFRIAccess access, FRICreationDescriptor Data) :
		FRIVertexBuffer(Size, Usage),
		Buffer(0),
		InputLayout(0),
		LayoutStride(0)
	{

		D3D11_BUFFER_DESC BufferDesc;

		if (Usage == 0)
		{
			BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		}
		else
		{
			BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		}

		BufferDesc.ByteWidth = Data.ByteSize;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		if (access == EFRIAccess::None)
		{
			BufferDesc.CPUAccessFlags = 0;
		}
		else if (access == EFRIAccess::CPUWrite)
		{
			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}


		BufferDesc.MiscFlags = 0;

		if (Data.DataArray)
		{
			D3D11_SUBRESOURCE_DATA BufferData;
			BufferData.pSysMem = Data.DataArray;
			BufferData.SysMemPitch = 0;
			BufferData.SysMemSlicePitch = 0;


			HRESULT hr = device->CreateBuffer(&BufferDesc, &BufferData, &Buffer);
		}
		else
		{

			HRESULT hr = device->CreateBuffer(&BufferDesc, nullptr, &Buffer);
		}


	}
};

struct FD3D11IndexBuffer : FRIIndexBuffer
{
	TComPtr<ID3D11Buffer> Buffer;

	FD3D11IndexBuffer(ID3D11Device* device, uint32 Size, uint32 Usage, FRICreationDescriptor Data) :
		FRIIndexBuffer(Size, Usage)
	{

		D3D11_BUFFER_DESC BufferDesc;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.ByteWidth = Data.ByteSize;
		BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		BufferDesc.CPUAccessFlags = 0;
		BufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA BufferData;
		BufferData.pSysMem = Data.DataArray;
		BufferData.SysMemPitch = 0;
		BufferData.SysMemSlicePitch = 0;


		HRESULT hr = device->CreateBuffer(&BufferDesc, &BufferData, &Buffer);

	}

};


struct FD3D11InstanceBuffer : FRIInstanceBuffer
{
	TComPtr<ID3D11Buffer> Buffer;
	
	uint32 LayoutStride;

	FD3D11InstanceBuffer(ID3D11Device* device, uint32 LayoutStride, uint32 Size, FRICreationDescriptor Data) :
		FRIInstanceBuffer(Size),
		LayoutStride(LayoutStride)
	{
		D3D11_BUFFER_DESC BufferDesc;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.ByteWidth = Data.ByteSize;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;



		if (Data.DataArray)
		{
			D3D11_SUBRESOURCE_DATA BufferData;
			BufferData.pSysMem = Data.DataArray;
			BufferData.SysMemPitch = 0;
			BufferData.SysMemSlicePitch = 0;


			HRESULT hr = device->CreateBuffer(&BufferDesc, &BufferData, &Buffer);
		}
		else
		{

			HRESULT hr = device->CreateBuffer(&BufferDesc, nullptr, &Buffer);
		}



	}
};

struct FD3D11TextureBase
{
	TComPtr<ID3D11Texture2D> Texture;
	TComPtr<ID3D11SamplerState> Sampler;
	DXGI_FORMAT Format;
	
	FD3D11TextureBase(DXGI_FORMAT format) :
		Format(format)
	{}

	virtual ~FD3D11TextureBase()
	{}
};

struct FD3D11Texture2D : FD3D11TextureBase, FD3D11ShaderResource, FRITexture2D
{
	FD3D11Texture2D(ID3D11Device* device, 
		uint32 width,
		uint32 height,
		uint32 sampleCount,
		DXGI_FORMAT format,
		FRICreationDescriptor Data = FRICreationDescriptor(NULL, 0),
		bool cpuWrite = false,
		bool bindDepth = false
	) :
		FRITexture2D(width, height, sampleCount),
		FD3D11ShaderResource(0),
		FD3D11TextureBase(format)
	{

		D3D11_TEXTURE2D_DESC TextureDesc;
		TextureDesc.Width = width;
		TextureDesc.Height = height;
		TextureDesc.MipLevels = TextureDesc.ArraySize = 1;
		TextureDesc.Format = format;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;


		if (cpuWrite)
		{
			TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
		}
		else
		{
			TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		}

		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		if (bindDepth)
		{
			TextureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		}


		if (!cpuWrite && !bindDepth)
		{
			TextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}

		TextureDesc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;

		if (cpuWrite)
		{
			TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		TextureDesc.MiscFlags = 0;


		if (Data.DataArray)
		{
			D3D11_SUBRESOURCE_DATA TextureData;
			TextureData.pSysMem = Data.DataArray;
			TextureData.SysMemPitch = Data.ByteSize / height;
			TextureData.SysMemSlicePitch = 0;

			device->CreateTexture2D(&TextureDesc, &TextureData, &Texture);
		}
		else
		{
			device->CreateTexture2D(&TextureDesc, nullptr, &Texture);

		}


		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;

		SrvDesc.Format = format;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SrvDesc.Texture2D.MostDetailedMip = 0;
		SrvDesc.Texture2D.MipLevels = 1;

		if (bindDepth)
		{
			SrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}

		device->CreateShaderResourceView(Texture, &SrvDesc, &ShaderResourceView);

		D3D11_SAMPLER_DESC SamplerDesc;

		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.MaxAnisotropy = 4.0f;
		SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamplerDesc.BorderColor[0] = 0;
		SamplerDesc.BorderColor[1] = 0;
		SamplerDesc.BorderColor[2] = 0;
		SamplerDesc.BorderColor[3] = 0;
		SamplerDesc.MinLOD = 0;
		SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		SamplerDesc.MipLODBias = 0;

		device->CreateSamplerState(&SamplerDesc, &Sampler);
	}

};



struct FD3D11Texture2DArray : FD3D11TextureBase, FD3D11ShaderResource, FRITexture2DArray
{
	FD3D11Texture2DArray(ID3D11Device* device,
		uint32 width,
		uint32 height,
		uint32 NumLayers,
		DXGI_FORMAT format
	) :
		FRITexture2DArray(width, height, NumLayers),
		FD3D11ShaderResource(0),
		FD3D11TextureBase(format)
	{

		D3D11_TEXTURE2D_DESC TextureDesc;
		TextureDesc.Width = width;
		TextureDesc.Height = height;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = NumLayers;
		TextureDesc.Format = format;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;

		HRESULT hr = device->CreateTexture2D(&TextureDesc, NULL, &Texture);


		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;

		SrvDesc.Format = format;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		SrvDesc.Texture2DArray.MostDetailedMip = 0;
		SrvDesc.Texture2DArray.ArraySize = NumLayers;
		SrvDesc.Texture2DArray.FirstArraySlice = 0;
		SrvDesc.Texture2DArray.MipLevels = 1;

		hr = device->CreateShaderResourceView(Texture, &SrvDesc, &ShaderResourceView);



		D3D11_SAMPLER_DESC SamplerDesc;

		SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDesc.MaxAnisotropy = 4.0f;
		SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamplerDesc.BorderColor[0] = 0;
		SamplerDesc.BorderColor[1] = 0;
		SamplerDesc.BorderColor[2] = 0;
		SamplerDesc.BorderColor[3] = 0;
		SamplerDesc.MinLOD = 0;
		SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		SamplerDesc.MipLODBias = 0;


		hr = device->CreateSamplerState(&SamplerDesc, &Sampler);

	}
};

struct FD3D11UniformBuffer : FRIUniformBuffer
{
	TComPtr<ID3D11Buffer> Buffer;

	FD3D11UniformBuffer(ID3D11Device* device,
		FRICreationDescriptor Data
	) : 
		FRIUniformBuffer(Data.ByteSize)
	{

		D3D11_BUFFER_DESC BufferDesc;
		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

		BufferDesc.ByteWidth = Data.ByteSize;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = 0;


		if (Data.DataArray)
		{
			D3D11_SUBRESOURCE_DATA BufferData;
			BufferData.pSysMem = Data.DataArray;
			BufferData.SysMemPitch = 0;
			BufferData.SysMemSlicePitch = 0;

			device->CreateBuffer(&BufferDesc, &BufferData, &Buffer);
		}
		else
		{
			device->CreateBuffer(&BufferDesc, NULL, &Buffer);
		}
	}
};


template<typename TShader>
struct TD3D11ShaderBase
{
	TComPtr<TShader> Shader;

	virtual ~TD3D11ShaderBase()
	{
	}
};


struct FD3D11VertexShader : FRIVertexShader, TD3D11ShaderBase<ID3D11VertexShader>
{
	FArray<byte> ByteCode;

	FD3D11VertexShader(ID3D11Device* device, const FArray<byte>& binCode) :
		ByteCode(binCode)
	{
		device->CreateVertexShader(binCode.Begin(), binCode.ByteSize(), NULL, &Shader);
	}
	uint32 GetResource() const
	{
		return 0;
	}
};

struct FD3D11PixelShader : FRIPixelShader, TD3D11ShaderBase<ID3D11PixelShader>
{
	FD3D11PixelShader(ID3D11Device* device, const FArray<byte>& binCode)
	{
		device->CreatePixelShader(binCode.Begin(), binCode.ByteSize(), NULL, &Shader);
	}
	uint32 GetResource() const
	{
		return 0;
	}
};

struct FD3D11GeometryShader : FRIGeometryShader, TD3D11ShaderBase<ID3D11GeometryShader>
{

	FD3D11GeometryShader(ID3D11Device* device, const FArray<byte>& binCode)
	{
		device->CreateGeometryShader(binCode.Begin(), binCode.ByteSize(), NULL, &Shader);
	}
	uint32 GetResource() const
	{
		return 0;
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


	FD3D11ShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor, const FArray<EFRIResourceShaderType>& types) : 
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
			case EFRIResourceShaderType::Vertex: vertexShader = static_cast<FD3D11VertexShader*>(descriptor.ShaderArray[i])->Shader; break;
			case EFRIResourceShaderType::Pixel: pixelShader = static_cast<FD3D11PixelShader*>(descriptor.ShaderArray[i])->Shader; break;
			case EFRIResourceShaderType::Geometry: geometryShader = static_cast<FD3D11GeometryShader*>(descriptor.ShaderArray[i])->Shader; break;
			}
		}
	}
};

struct FD3D11FrameBuffer : FRIFrameBuffer
{
	TComPtr<ID3D11RenderTargetView>* RenderTargetViews;
	TComPtr<ID3D11DepthStencilView> DepthStencilView;
	TComPtr<ID3D11ShaderResourceView> DSVShaderResourceView;

	uint32 NumViews;

	FD3D11Texture2D* DepthTexture;

	FD3D11FrameBuffer(ID3D11Device* device, FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthBuffer) : 
		FRIFrameBuffer(0, 0),
		NumViews(textureAttachments.Length()),
		RenderTargetViews(0)
	{

		RenderTargetViews = new TComPtr<ID3D11RenderTargetView>[NumViews];

		FArray<IVector2> RtvDimensions;

		for (int i = 0; i < NumViews; i++)
		{
			D3D11_RENDER_TARGET_VIEW_DESC RtvDesc;
			ZeroMemory(&RtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));


			FD3D11Texture2D* fdxResource = static_cast<FD3D11Texture2D*>(textureAttachments[i].Param2D);
			RtvDesc.Format = fdxResource->Format;
			RtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			RtvDesc.Texture2D.MipSlice = 0;

			RtvDimensions.Add(IVector2(fdxResource->Width, fdxResource->Height));

			device->CreateRenderTargetView(fdxResource->Texture, &RtvDesc, &RenderTargetViews[i]);

		}

		if (enableDepthBuffer)
		{
			CreateDepthBuffer(device, RtvDimensions[0]);
		}

	}

	FD3D11FrameBuffer(ID3D11Device* device, FRIFrameBufferArrayAttachment textureAttachment, bool enableDepthBuffer) :
		FRIFrameBuffer(0, 0),
		NumViews(0),
		RenderTargetViews(0)
	{

		FD3D11Texture2DArray* fdxResource = static_cast<FD3D11Texture2DArray*>(textureAttachment.Param2DArray);
		NumViews = fdxResource->NumLayers;
		RenderTargetViews = new TComPtr<ID3D11RenderTargetView>[NumViews];

		IVector2 RtvDimensions(fdxResource->Width, fdxResource->Height);


		D3D11_RENDER_TARGET_VIEW_DESC RtvDesc;
		ZeroMemory(&RtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		RtvDesc.Format = fdxResource->Format;
		RtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		RtvDesc.Texture2DArray.MipSlice = 0;
		RtvDesc.Texture2DArray.ArraySize = 1;

		for (int i = 0; i < NumViews; i++)
		{
			RtvDesc.Texture2DArray.FirstArraySlice = D3D11CalcSubresource(0, i, 1);

			device->CreateRenderTargetView(fdxResource->Texture, &RtvDesc, &RenderTargetViews[i]);
		}

		if (enableDepthBuffer)
		{
			CreateDepthBuffer(device, RtvDimensions);
		}

	}

	void CreateDepthBuffer(ID3D11Device* device, IVector2 Dimensions)
	{


		DepthTexture = new FD3D11Texture2D(device, Dimensions.x, Dimensions.y, 0, DXGI_FORMAT_R32_TYPELESS, FRICreationDescriptor(0, 0), false, true);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.Flags = 0;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;


		HRESULT hr = device->CreateDepthStencilView(DepthTexture->Texture, &dsvDesc, &DepthStencilView);


		/*ID3D11Texture2D* pDepthStencil = NULL;
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = Dimensions.x;
		descDepth.Height = Dimensions.y;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		HRESULT hr = device->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.Flags = 0;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;


		hr = device->CreateDepthStencilView(pDepthStencil, &dsvDesc, &DepthStencilView);


		D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;
		ZeroMemory(&SrvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		SrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SrvDesc.Texture2D.MipLevels = 1;
		SrvDesc.Texture2D.MostDetailedMip = 0;

		device->CreateShaderResourceView(pDepthStencil, &SrvDesc, &DSVShaderResourceView);*/


	}

	FRITexture2D* GetDepthBuffer()
	{
		return DepthTexture;
	}
};

struct FD3D11VertexDeclaration : FRIVertexDeclaration
{
	TComPtr<ID3D11InputLayout> InputLayout;
	FArray<uint32> LayoutStrides;

	FD3D11VertexDeclaration(
		
		ID3D11Device* device, 
		const FArray<FRIVertexDeclarationDesc>& DescArray, 
		FD3D11VertexShader* shaderSignature
	) : 
		FRIVertexDeclaration(DescArray)
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
				const FRIVertexDeclarationComponent& Component = DescArray[layoutIndex].Components[i];

				D3D11_INPUT_ELEMENT_DESC ElemDesc;

				uint32 componentCount = Component.Length;
				DXGI_FORMAT format;

				switch (componentCount)
				{
				case 1:
					format = DXGI_FORMAT_R32_FLOAT;
					break;
				case 2:
					format = DXGI_FORMAT_R32G32_FLOAT;
					break;
				case 3:
					format = DXGI_FORMAT_R32G32B32_FLOAT;
					break;
				case 4:
					format = DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
				default:
					format = DXGI_FORMAT_UNKNOWN;
					break;
				}

				if (Component.Type == EFRIVertexDeclerationAttributeType::Int)
				{
					format = (DXGI_FORMAT)(format + 1); // Hack for making the attribute of type int
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

		for (int i = 0; i < NumLayoutElems; i++)
		{
			printf("%s %d\n", vertexLayout[i].SemanticName, vertexLayout[i].SemanticIndex);
		}

		device->CreateInputLayout(vertexLayout.Begin(), NumLayoutElems, shaderSignature->ByteCode.Begin(), shaderSignature->ByteCode.ByteSize(), &InputLayout);

	}
};


struct FD3D11RasterizerState : FRIRasterizerState
{
	TComPtr<ID3D11RasterizerState> RasterizerState;

	FD3D11RasterizerState(ID3D11Device* device,
		D3D11_CULL_MODE cullMode,
		D3D11_FILL_MODE fillMode
		)
	{
		D3D11_RASTERIZER_DESC RasterizerDesc;
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

		device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
	}
};


struct FD3D11BlendState : FRIBlendState
{
	TComPtr<ID3D11BlendState> BlendState;
	
	FD3D11BlendState(
		ID3D11Device* device,

		D3D11_BLEND SrcBlend,
		D3D11_BLEND DstBlend
		)
	{
		D3D11_BLEND_DESC BlendDesc;
		ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));

		BlendDesc.RenderTarget[0].BlendEnable = TRUE;
		BlendDesc.RenderTarget[0].SrcBlend = SrcBlend;
		BlendDesc.RenderTarget[0].DestBlend = DstBlend;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		device->CreateBlendState(&BlendDesc, &BlendState);
	}
};


struct FD3D11DepthStencilState : FRIDepthStencilState
{
	TComPtr<ID3D11DepthStencilState> DepthStencilState;

	FD3D11DepthStencilState(
		ID3D11Device* device,
		EFRIBool DepthEnable,
		EFRIBool StencilEnable

	)
	{
		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
		ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		DepthStencilDesc.DepthEnable = (BOOL)DepthEnable;
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		DepthStencilDesc.StencilEnable = (BOOL)StencilEnable;

		device->CreateDepthStencilState(&DepthStencilDesc, &DepthStencilState);

	}
};