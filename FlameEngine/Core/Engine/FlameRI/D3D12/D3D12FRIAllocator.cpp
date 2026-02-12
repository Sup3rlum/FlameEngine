#include "D3D12FRICommon.h"
#include "D3D12FRIContext.h"

#include "Core/Framework/IO/FileStream.h"


const uint32 ZEROOFFSET[] = { 0,0,0,0,0 };


FRIVertexShader* D3D12FRIDynamicAllocator::CreateVertexShader(const FArray<byte>& binCode)
{
	return NULL;// new FD3D11VertexShader(D3DFRI->Device, binCode);
}
FRIPixelShader* D3D12FRIDynamicAllocator::CreatePixelShader(const FArray<byte>& binCode)
{
	return NULL;// new FD3D11PixelShader(D3DFRI->Device, binCode);
}
FRIGeometryShader* D3D12FRIDynamicAllocator::CreateGeometryShader(const FArray<byte>& binCode)
{
	return NULL;// new FD3D11GeometryShader(D3DFRI->Device, binCode);
}
FRIHullShader* D3D12FRIDynamicAllocator::CreateHullShader(const FArray<byte>& binCode)
{
	return NULL;// new FD3D11HullShader(D3DFRI->Device, binCode);
}
FRIDomainShader* D3D12FRIDynamicAllocator::CreateDomainShader(const FArray<byte>& binCode)
{
	return NULL;// new FD3D11DomainShader(D3DFRI->Device, binCode);
}
FRIComputeShader* D3D12FRIDynamicAllocator::CreateComputeShader(const FArray<byte>& binCode)
{
	return NULL;// new FD3D11ComputeShader(D3DFRI->Device, binCode);
}



FRIShaderPipeline* D3D12FRIDynamicAllocator::CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor)
{
	return NULL;
}

FRIShaderPipeline* D3D12FRIDynamicAllocator::CreateShaderPipeline(const ShaderLibraryModule& shaderModule)
{
	FArray<FRIShaderBase*> shaderArray;
	FArray<EFRIShaderType> typesArray;

	for (const auto& part : shaderModule.Parts)
	{
		FRIShaderBase* shaderResource = NULL;

		switch (part.Key)
		{
		case EFRIShaderType::Vertex: shaderResource = CreateVertexShader(part.Value.Memory); break;
		case EFRIShaderType::Pixel: shaderResource = CreatePixelShader(part.Value.Memory); break;
		case EFRIShaderType::Geometry: shaderResource = CreateGeometryShader(part.Value.Memory); break;
		case EFRIShaderType::Hull: shaderResource = CreateHullShader(part.Value.Memory); break;
		case EFRIShaderType::Domain: shaderResource = CreateDomainShader(part.Value.Memory); break;
		case EFRIShaderType::Compute: shaderResource = CreateComputeShader(part.Value.Memory); break;

		}

		shaderArray.Add(shaderResource);
		typesArray.Add(part.Key);
	}

	return NULL;// new FD3D11ShaderPipeline(FRIShaderPipelineCreationDescriptor(shaderArray.Length(), shaderArray.Begin()), typesArray);
}

FRITexture2D* D3D12FRIDynamicAllocator::CreateTexture2D(
	uint32 width,
	uint32 height,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[]
)
{

	return new FD3D12Texture2D(GetParentDevice(), width, height, mipLevels, EDX12FormatProxyEnum(format), resourceDescriptor);
}
FRITexture3D* D3D12FRIDynamicAllocator::CreateTexture3D(
	uint32 width,
	uint32 height,
	uint32 depth,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[]
)
{

	return new FD3D12Texture3D(GetParentDevice(), width, height, depth, mipLevels, EDX12FormatProxyEnum(format), resourceDescriptor);
}


FRITexture2DArray* D3D12FRIDynamicAllocator::CreateTexture2DArray(
	uint32 width,
	uint32 height,
	uint32 numLayers,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[]
)
{

	return new FD3D12Texture2DArray(GetParentDevice(), width, height, numLayers, mipLevels, EDX12FormatProxyEnum(format), resourceDescriptor);
}

FRITextureCubeMap* D3D12FRIDynamicAllocator::CreateTextureCubeMap(
	uint32 width,
	uint32 height,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[]
)
{
	if (width != height)
	{
		return nullptr;
	}

	return new FD3D12TextureCubeMap(GetParentDevice(), width, mipLevels, EDX12FormatProxyEnum(format), resourceDescriptor);
}

FRIConstantBuffer* D3D12FRIDynamicAllocator::CreateConstantBuffer(
	uint32 ByteSize,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor
)
{

	return new FD3D12ConstantBuffer(GetParentDevice(), ByteSize, resourceDescriptor);
}


FRIIndexBuffer* D3D12FRIDynamicAllocator::CreateIndexBuffer(
	uint32 IndexCount,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor
)
{
	return new FD3D12IndexBuffer(GetParentDevice(), IndexCount, sizeof(int), resourceDescriptor);

}
FRIVertexBuffer* D3D12FRIDynamicAllocator::CreateVertexBuffer(
	uint32 VertexCount,
	uint32 VertexSize,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor
)
{
	return new FD3D12VertexBuffer(GetParentDevice(), VertexCount, VertexSize, resourceDescriptor);
}

FRIInstanceBuffer* D3D12FRIDynamicAllocator::CreateInstanceBuffer(
	uint32 InstanceCount,
	uint32 InstanceSize,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor
)
{
	return NULL;
}



FRIComputeBuffer* D3D12FRIDynamicAllocator::CreateComputeBuffer(
	uint32 StructureCount,
	uint32 StructureStride,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor)
{

	return new FD3D12ComputeBuffer(GetParentDevice(), StructureCount, StructureStride, resourceDescriptor);
}

FRIRenderTargetView* D3D12FRIDynamicAllocator::CreateRenderTargetView(FRITexture2D* Target, uint32 targetMipLevel)
{
	auto FD3D12Tex = DX12::Cast<FD3D12Texture2D>(Target);
	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMem(RTVDesc);
	RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	return new FD3D12RenderTargetView(GetParentDevice(), RTVDesc, FD3D12Tex->TargetResource);
}
FRIRenderTargetView* D3D12FRIDynamicAllocator::CreateRenderTargetView(FRITexture2DArray* Target, uint32 targetLayer, uint32 targetMipLevel)
{

	auto FD3D12Tex = DX12::Cast<FD3D12Texture2DArray>(Target);
	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMem(RTVDesc);

	RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
	RTVDesc.Texture2DArray.ArraySize = 1;
	RTVDesc.Texture2DArray.FirstArraySlice = D3D12CalcSubresource(targetMipLevel, targetLayer, 0, FD3D12Tex->MipLevels, FD3D12Tex->NumLayers);
	RTVDesc.Texture2DArray.MipSlice = targetMipLevel;

	return new FD3D12RenderTargetView(GetParentDevice(), RTVDesc, FD3D12Tex->TargetResource);
}

FRIRenderTargetView* D3D12FRIDynamicAllocator::CreateRenderTargetView(FRITextureCubeMap* Target, uint32 targetLayer, uint32 targetMipLevel)
{
	auto FD3D12Tex = DX12::Cast<FD3D12TextureCubeMap>(Target);
	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
	ZeroMem(RTVDesc);

	RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
	RTVDesc.Texture2DArray.ArraySize = 1;
	RTVDesc.Texture2DArray.FirstArraySlice = D3D12CalcSubresource(targetMipLevel, targetLayer, 0, FD3D12Tex->MipLevels, 6);
	RTVDesc.Texture2DArray.MipSlice = targetMipLevel;

	return new FD3D12RenderTargetView(GetParentDevice(), RTVDesc, FD3D12Tex->TargetResource);
}

FRIShaderResourceView* D3D12FRIDynamicAllocator::CreateShaderResourceView(FRITexture2D* Target)
{
	auto FD3D12Tex = DX12::Cast<FD3D12Texture2D>(Target);
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMem(SRVDesc);
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = -1;

	if (FD3D12Tex->Format == DXGI_FORMAT_D32_FLOAT)
	{
		SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}

	return new FD3D12ShaderResourceView(GetParentDevice(), SRVDesc, FD3D12Tex->TargetResource);
}
FRIShaderResourceView* D3D12FRIDynamicAllocator::CreateShaderResourceView(FRITexture2DArray* Target)
{
	auto FD3D12ArrayTex = DX12::Cast<FD3D12Texture2DArray>(Target);
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMem(SRVDesc);
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	SRVDesc.Texture2DArray.ArraySize = FD3D12ArrayTex->NumLayers;
	SRVDesc.Texture2DArray.MostDetailedMip = 0;
	SRVDesc.Texture2DArray.MipLevels = -1;

	if (FD3D12ArrayTex->Format == DXGI_FORMAT_D32_FLOAT)
	{
		SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}

	return new FD3D12ShaderResourceView(GetParentDevice(), SRVDesc, FD3D12ArrayTex->TargetResource);
}

FRIShaderResourceView* D3D12FRIDynamicAllocator::CreateShaderResourceView(FRITexture3D* Target)
{
	auto FD3D12ArrayTex = DX12::Cast<FD3D12Texture3D>(Target);
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMem(SRVDesc);
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
	SRVDesc.Texture3D.MipLevels = -1;
	SRVDesc.Texture3D.MostDetailedMip = 0;

	if (FD3D12ArrayTex->Format == DXGI_FORMAT_D32_FLOAT)
	{
		SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}

	return new FD3D12ShaderResourceView(GetParentDevice(), SRVDesc, FD3D12ArrayTex->TargetResource);
}

FRIConstantBufferView* D3D12FRIDynamicAllocator::CreateConstantBufferView(FRIConstantBuffer* Target)
{
	auto FD3D12Buffer = DX12::Cast<FD3D12ConstantBuffer>(Target);
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
	ZeroMem(CBVDesc);
	CBVDesc.BufferLocation = FD3D12Buffer->TargetResource->Resource->GetGPUVirtualAddress();
	CBVDesc.SizeInBytes = (FD3D12Buffer->ByteSize + 255) & ~255;

	return new FD3D12ConstantBufferView(GetParentDevice(), CBVDesc);
}

FRIDepthStencilView* D3D12FRIDynamicAllocator::CreateDepthStencilView(FRITexture2D* Target)
{
	auto FD3D12Buffer = DX12::Cast<FD3D12Texture2D>(Target);
	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	ZeroMem(DSVDesc);
	DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	return new FD3D12DepthStencilView(GetParentDevice(), DSVDesc, FD3D12Buffer->TargetResource);
}


FRIUnorderedAccessView* D3D12FRIDynamicAllocator::CreateUnorderedAccessView(FRIConstantBuffer* Target)
{
	return NULL;
}
FRIUnorderedAccessView* D3D12FRIDynamicAllocator::CreateUnorderedAccessView(FRIVertexBuffer* Target)
{
	auto FD3D12Buffer = DX12::Cast<FD3D12VertexBuffer>(Target);
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMem(UAVDesc);
	UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.NumElements = FD3D12Buffer->VertexCount;
	UAVDesc.Buffer.StructureByteStride = FD3D12Buffer->VertexSize;

	return new FD3D12UnorderedAccessView(GetParentDevice(), UAVDesc, FD3D12Buffer->TargetResource);
}

FRIUnorderedAccessView* D3D12FRIDynamicAllocator::CreateUnorderedAccessView(FRIIndexBuffer* Target)
{
	auto FD3D12Buffer = DX12::Cast<FD3D12IndexBuffer>(Target);
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMem(UAVDesc);
	UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.NumElements = FD3D12Buffer->IndexCount;
	UAVDesc.Buffer.StructureByteStride = sizeof(uint32);

	return new FD3D12UnorderedAccessView(GetParentDevice(), UAVDesc, FD3D12Buffer->TargetResource);
}

FRIUnorderedAccessView* D3D12FRIDynamicAllocator::CreateUnorderedAccessView(FRIComputeBuffer* Target)
{
	auto FD3D12Buffer = DX12::Cast<FD3D12ComputeBuffer>(Target);
	D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMem(UAVDesc);
	UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.NumElements = FD3D12Buffer->StructureCount;
	UAVDesc.Buffer.StructureByteStride = FD3D12Buffer->StructureStride;

	return new FD3D12UnorderedAccessView(GetParentDevice(), UAVDesc, FD3D12Buffer->TargetResource);
}

FRIRasterizerState* D3D12FRIDynamicAllocator::CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode)
{
	return NULL;
}

FRIBlendState* D3D12FRIDynamicAllocator::CreateBlendState(
	EFRIBlend srcBlend,
	EFRIBlend dstBlend,
	EFRIBlend srcAlphaBlend,
	EFRIBlend dstAlphaBlend
)
{
	return NULL;
}
FRIDepthStencilState* D3D12FRIDynamicAllocator::CreateDepthStencilState(EFRIBool depth, EFRIBool stencil)
{
	return NULL;
}

FRISamplerState* D3D12FRIDynamicAllocator::CreateSamplerState(EFRITextureFilter filter, EFRITextureAddress addressU, EFRITextureAddress addressV, EFRITextureAddress addressW, Color32 borderColor)
{
	return NULL;
}

FORCEINLINE D3D12_SHADER_BYTECODE CreateD3D12ShaderBytecode(const ShaderLibraryModulePart& part)
{
	return D3D12_SHADER_BYTECODE{ .pShaderBytecode = part.Memory.Begin(), .BytecodeLength = part.Memory.Length() };
}

FRIPipelineStateObject* D3D12FRIDynamicAllocator::CreatePipelineStateObject(
	const ShaderLibraryModule& shaderModule,
	FRIInputLayout inputLayout,
	FArray<FRIRootParameter> RootParameters,
	FArray<FRIStaticSampler> StaticSamplers,
	FArray<EFRITextureFormat> RTVFormats,
	FRIRasterizerState RasterizerState,
	FRIDepthStencilState DepthStencilState,
	FRIBlendState BlendState
)
{
	auto RootSignature = new FD3D12RootSignature(GetParentDevice(), RootParameters, StaticSamplers);

	D3D12_SHADER_BYTECODE VS = { 0 };
	D3D12_SHADER_BYTECODE PS = { 0 };
	D3D12_SHADER_BYTECODE HS = { 0 };
	D3D12_SHADER_BYTECODE DS = { 0 };
	D3D12_SHADER_BYTECODE GS = { 0 };

	for (const auto& part : shaderModule.Parts)
	{
		switch (part.Key)
		{
			case EFRIShaderType::Vertex:	VS = CreateD3D12ShaderBytecode(part.Value); break;
			case EFRIShaderType::Pixel:		PS = CreateD3D12ShaderBytecode(part.Value); break;
			case EFRIShaderType::Geometry:	GS = CreateD3D12ShaderBytecode(part.Value); break;
			case EFRIShaderType::Hull:		HS = CreateD3D12ShaderBytecode(part.Value); break;
			case EFRIShaderType::Domain:	DS = CreateD3D12ShaderBytecode(part.Value); break;
		}
	}

	return new FD3D12GraphicsPipelineStateObject(
		GetParentDevice(),
		inputLayout,
		RootSignature,
		VS,
		PS,
		GS,
		HS,
		DS,
		RTVFormats,
		RasterizerState,
		DepthStencilState,
		BlendState
		);
}

FRIPipelineStateObject* D3D12FRIDynamicAllocator::CreateComputePipelineStateObject(
	FArray<FRIRootParameter> RootParameters,
	const ShaderLibraryModule& shaderModule
)
{
	auto stuff = new FD3D12RootSignature(GetParentDevice(), RootParameters, {});
	D3D12_SHADER_BYTECODE CS = { 0 };

	for (const auto& part : shaderModule.Parts)
	{
		switch (part.Key)
		{
		case EFRIShaderType::Compute:	CS = CreateD3D12ShaderBytecode(part.Value); break;

		}
	}

	return new FD3D12ComputePipelineStateObject(
		GetParentDevice(),
		stuff,
		CS
	);
}