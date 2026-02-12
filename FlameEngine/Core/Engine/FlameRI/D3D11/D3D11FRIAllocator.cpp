#include "D3D11FRIAllocator.h"
#include "D3D11FRIContext.h"
#include "D3D11FRIResource.h"

#include "Core/Framework/IO/FileStream.h"


const uint32 ZEROOFFSET[] = { 0,0,0,0,0 };


D3D11_FILL_MODE FRID3DFillModeEnums[2]
{
	D3D11_FILL_SOLID,
	D3D11_FILL_WIREFRAME
};


D3D11_CULL_MODE FRID3DCullModeEnums[3]
{
	D3D11_CULL_NONE,
	D3D11_CULL_FRONT,
	D3D11_CULL_BACK
};

D3D11_BLEND FRID3DBlendEnums[6]
{
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA
};

ID3D11RenderTargetView* FRID3DEmptyRTV[16] = { 0 };
ID3D11ShaderResourceView* FRID3DEmptySRV[16] = { 0 };
ID3D11UnorderedAccessView* FRID3DEmptyUAV[16] = { 0 };


FRIVertexShader* D3D11FRIDynamicAllocator::CreateVertexShader(const FArray<byte>& binCode)
{
	return new FD3D11VertexShader(D3DFRI->Device, binCode);
}
FRIPixelShader* D3D11FRIDynamicAllocator::CreatePixelShader(const FArray<byte>& binCode)
{
	return new FD3D11PixelShader(D3DFRI->Device, binCode);
}
FRIGeometryShader* D3D11FRIDynamicAllocator::CreateGeometryShader(const FArray<byte>& binCode)
{
	return new FD3D11GeometryShader(D3DFRI->Device, binCode);
}
FRIHullShader* D3D11FRIDynamicAllocator::CreateHullShader(const FArray<byte>& binCode)
{
	return new FD3D11HullShader(D3DFRI->Device, binCode);
}
FRIDomainShader* D3D11FRIDynamicAllocator::CreateDomainShader(const FArray<byte>& binCode)
{
	return new FD3D11DomainShader(D3DFRI->Device, binCode);
}
FRIComputeShader* D3D11FRIDynamicAllocator::CreateComputeShader(const FArray<byte>& binCode)
{
	return new FD3D11ComputeShader(D3DFRI->Device, binCode);
}



FRIShaderPipeline* D3D11FRIDynamicAllocator::CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor)
{
	return NULL;
}

FRIShaderPipeline* D3D11FRIDynamicAllocator::CreateShaderPipeline(const ShaderLibraryModule& shaderModule)
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

	return new FD3D11ShaderPipeline(FRIShaderPipelineCreationDescriptor(shaderArray.Length(), shaderArray.Begin()), typesArray);
}

FRITexture2D* D3D11FRIDynamicAllocator::CreateTexture2D(
	uint32 width,
	uint32 height,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[]
)
{
	DXGI_FORMAT gpuformat = EDX11FormatProxyEnum(format);
	D3D11_USAGE Usage = Access == EFRIAccess::Write ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	UINT access = D3DGetAccess(Access);
	UINT Bind = D3D11_BIND_SHADER_RESOURCE;
		
	if (Access != EFRIAccess::Write)
		Bind |= D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;

	UINT Misc = 0;// D3D11_RESOURCE_MISC_GENERATE_MIPS;

	auto pFdxTex2D = new FD3D11Texture2D(D3DFRI->Device, width, height, mipLevels, gpuformat, Usage, access, Bind, Misc, resourceDescriptor);

	if (Access != EFRIAccess::Write)
		pFdxTex2D->CreateUAV(D3DFRI->Device);

	pFdxTex2D->CreateSRV(D3DFRI->Device);

	return pFdxTex2D;
}
FRITexture3D* D3D11FRIDynamicAllocator::CreateTexture3D(
	uint32 width,
	uint32 height,
	uint32 depth,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[]
)
{
	DXGI_FORMAT gpuformat = EDX11FormatProxyEnum(format);
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT access = D3DGetAccess(Access);
	UINT Bind = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
	UINT Misc = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	auto pFdxTex3D = new FD3D11Texture3D(D3DFRI->Device, width, height, depth, mipLevels, gpuformat, Usage, access, Bind, Misc, resourceDescriptor);
	
	pFdxTex3D->CreateSRV(D3DFRI->Device);
	pFdxTex3D->CreateUAV(D3DFRI->Device);

	return pFdxTex3D;
}


FRITexture2DArray* D3D11FRIDynamicAllocator::CreateTexture2DArray(
	uint32 width,
	uint32 height,
	uint32 numLayers,
	uint32 mipLevels,
	EFRIAccess Access,
	EFRITextureFormat format,
	FRICreationDescriptor resourceDescriptor[]
)
{
	DXGI_FORMAT gpuformat = EDX11FormatProxyEnum(format);
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT access = D3DGetAccess(Access);
	UINT Bind = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
	UINT Misc = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	auto pFdxTex2DArray = new FD3D11Texture2DArray(D3DFRI->Device, width, height, numLayers, mipLevels, gpuformat, Usage, access, Bind, Misc, resourceDescriptor);

	pFdxTex2DArray->CreateSRV(D3DFRI->Device);
	pFdxTex2DArray->CreateUAV(D3DFRI->Device);

	return pFdxTex2DArray;
}

FRITextureCubeMap* D3D11FRIDynamicAllocator::CreateTextureCubeMap(
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

	DXGI_FORMAT gpuformat = EDX11FormatProxyEnum(format);
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	UINT access = D3DGetAccess(Access);
	UINT Bind = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
	UINT Misc = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	auto pFdxTexCube = new FD3D11TextureCubeMap(D3DFRI->Device, width, height, mipLevels, gpuformat, Usage, access, Bind, Misc, resourceDescriptor);

	pFdxTexCube->CreateSRV(D3DFRI->Device);
	pFdxTexCube->CreateUAV(D3DFRI->Device);

	return pFdxTexCube;
}

FRIConstantBuffer* D3D11FRIDynamicAllocator::CreateConstantBuffer(
	uint32 ByteSize,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor
)
{

	return new FD3D11ConstantBuffer(D3DFRI->Device, ByteSize, Access, Usage, resourceDescriptor);
}


FRIIndexBuffer* D3D11FRIDynamicAllocator::CreateIndexBuffer(
	uint32 IndexCount, 
	EFRIAccess Access, 
	EFRIUsage Usage, 
	FRICreationDescriptor resourceDescriptor
)
{
	return new FD3D11IndexBuffer(D3DFRI->Device, IndexCount, Access, Usage, resourceDescriptor);

}
FRIVertexBuffer* D3D11FRIDynamicAllocator::CreateVertexBuffer(
	uint32 VertexCount,
	uint32 VertexSize,
	EFRIAccess Access, 
	EFRIUsage Usage, 
	FRICreationDescriptor resourceDescriptor
)
{
	return new FD3D11VertexBuffer(D3DFRI->Device, VertexCount, VertexSize, Access, Usage, resourceDescriptor);
}

FRIInstanceBuffer* D3D11FRIDynamicAllocator::CreateInstanceBuffer(
	uint32 InstanceCount, 
	uint32 InstanceSize, 
	EFRIAccess Access, 
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor
)
{
	return new FD3D11InstanceBuffer(D3DFRI->Device, InstanceCount, InstanceSize, Access, Usage, resourceDescriptor);
}



FRIComputeBuffer* D3D11FRIDynamicAllocator::CreateComputeBuffer(
	uint32 StructureCount,
	uint32 StructureStride,
	EFRIAccess Access,
	EFRIUsage Usage,
	FRICreationDescriptor resourceDescriptor)
{

	auto pFdxCompBuffer = new FD3D11ComputeBuffer(D3DFRI->Device, StructureCount, StructureStride, Access, Usage, resourceDescriptor);

	pFdxCompBuffer->CreateUAV(D3DFRI->Device);
	return pFdxCompBuffer;
}

/*
FRIRenderTargetView* D3D11FRIDynamicAllocator::CreateRenderTargetView(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer)
{
	return new FD3D11RenderTargetView(D3DFRI->Device, textureAttachments, enableDepthRenderBuffer);
}
FRIRenderTargetView* D3D11FRIDynamicAllocator::CreateRenderTargetView(FRIFrameBufferArrayAttachment textureAttachment, bool enableDepthRenderBuffer, uint32 mipLevel)
{
	return new FD3D11RenderTargetView(D3DFRI->Device, textureAttachment, enableDepthRenderBuffer, mipLevel);
}*/

FRIRenderTargetView* D3D11FRIDynamicAllocator::CreateRenderTargetView(
	FRITexture2D* target,
	uint32 targetMipLevel
)
{
	return NULL;
}

FRIRenderTargetView* D3D11FRIDynamicAllocator::CreateRenderTargetView(
	FRITexture2DArray* target,
	uint32 targetLayer,
	uint32 targetMipLayer
)
{
	return NULL;

}

FRIRenderTargetView* D3D11FRIDynamicAllocator::CreateRenderTargetView(
	FRITextureCubeMap* target,
	uint32 targetSide,
	uint32 targetMipLayer
)
{
	return NULL;

}

/*
FRIInputLayout* D3D11FRIDynamicAllocator::CreateInputLayout(FArray<FRIInputDesc> DeclCompArray, FRIVertexShader* signatureShader)
{
	return NULLnew FD3D11InputLayout(D3DFRI->Device, DeclCompArray, signatureShader->As<FD3D11VertexShader>());
}*/

void D3D11FRIDynamicAllocator::AttachInputLayout(FRIVertexBuffer* geometry, FRIInputLayout* declaration)
{
	/*auto fdxVertexDeclaration = declaration->As<FD3D11InputLayout>();
	auto fdxGeometry = geometry->As<FD3D11VertexBuffer>();

	fdxGeometry->InputLayout = fdxVertexDeclaration->InputLayout;
	fdxGeometry->LayoutStride = fdxVertexDeclaration->LayoutStrides[0];*/
}

FRIRasterizerState* D3D11FRIDynamicAllocator::CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode)
{
	return new FD3D11RasterizerState(D3DFRI->Device, FRID3DCullModeEnums[(uint32)cullMode], FRID3DFillModeEnums[(uint32)fillmode]);
}

FRIBlendState* D3D11FRIDynamicAllocator::CreateBlendState(
	EFRIBlend srcBlend, 
	EFRIBlend dstBlend,
	EFRIBlend srcAlphaBlend,
	EFRIBlend dstAlphaBlend
)
{
	return new FD3D11BlendState(D3DFRI->Device, 
		FRID3DBlendEnums[(uint32)srcBlend], 
		FRID3DBlendEnums[(uint32)dstBlend],
		FRID3DBlendEnums[(uint32)srcAlphaBlend],
		FRID3DBlendEnums[(uint32)dstAlphaBlend]
	);
}
FRIDepthStencilState* D3D11FRIDynamicAllocator::CreateDepthStencilState(EFRIBool depth, EFRIBool stencil)
{
	return new FD3D11DepthStencilState(D3DFRI->Device, depth, stencil);
}

FRISamplerState* D3D11FRIDynamicAllocator::CreateSamplerState(EFRITextureFilter filter, EFRITextureAddress addressU, EFRITextureAddress addressV, EFRITextureAddress addressW, Color32 borderColor)
{
	return new FD3D11SamplerState(D3DFRI->Device, filter, addressU, addressV, addressW, borderColor);
}


void D3D11FRIDynamicAllocator::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	D3D11_VIEWPORT viewport;

	viewport.Width = width;
	viewport.Height = height;
	viewport.TopLeftX = x;
	viewport.TopLeftY = y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	D3DFRI->DeviceContext->RSSetViewports(1, &viewport);
}
void D3D11FRIDynamicAllocator::BindRenderTargets(FRIRenderTargetView* frameBuffer)
{
	/*FD3D11RenderTargetView* fdxFrameBuffer = frameBuffer->As<FD3D11RenderTargetView>();

	D3DFRI->DeviceContext->OMSetRenderTargets(
		fdxFrameBuffer->NumViews,
		fdxFrameBuffer->RenderTargetViews[0].GetAddressOf(),
		fdxFrameBuffer->DepthStencilView.Get()
	);*/
}
void D3D11FRIDynamicAllocator::UnbindFrameBuffer()
{
	D3DFRI->DeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(
		8,
		FRID3DEmptyRTV,
		NULL,
		9,
		8,
		FRID3DEmptyUAV,
		NULL
	);

	D3DFRI->DeviceContext->OMSetRenderTargets(1, &D3DFRI->RenderTargetView, D3DFRI->DepthStencilView.Get());
}

void D3D11FRIDynamicAllocator::ClearRenderTarget(FRIRenderTargetView* buffer, Color32 color, FRIUnorderedAccessView** uavs, uint32 uavNum)
{
	/*
	if (buffer)
	{
		BindRenderTargets(buffer);
		FD3D11RenderTargetView* fdxFrameBuffer = buffer->As<FD3D11FrameBuffer>();

		for (int i = 0; i < fdxFrameBuffer->NumViews; i++)
		{
			D3DFRI->DeviceContext->ClearRenderTargetView(fdxFrameBuffer->RenderTargetViews[i].Get(), &color.r);
		}
		if (fdxFrameBuffer->DepthStencilView.Get())
		{
			D3DFRI->DeviceContext->ClearDepthStencilView(fdxFrameBuffer->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}
	else
	{
		D3DFRI->DeviceContext->OMSetRenderTargets(1, &D3DFRI->RenderTargetView, D3DFRI->DepthStencilView.Get());

		D3DFRI->DeviceContext->ClearRenderTargetView(D3DFRI->RenderTargetView.Get(), &color.r);
		D3DFRI->DeviceContext->ClearDepthStencilView(D3DFRI->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}*/
}
/*
void D3D11FRIDynamicAllocator::ClearBufferAndSetUnorderedAccessViews(FRIFrameBuffer* buffer, Color32 color, FRIUnorderedAccessView** uavs, uint32 uavNum)
{
	ID3D11UnorderedAccessView** d3d11uavs = new ID3D11UnorderedAccessView * [uavNum];

	for (int i = 0; i < uavNum; i++)
	{
		d3d11uavs[i] = uavs[i]->As<FD3D11UnorderedAccessView>()->UnorderedAccessView.Get();
	}

	FD3D11FrameBuffer* fdxFrameBuffer = buffer->As<FD3D11FrameBuffer>();

	D3DFRI->DeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(
		fdxFrameBuffer->NumViews,
		&fdxFrameBuffer->RenderTargetViews[0], 
		fdxFrameBuffer->DepthStencilView.Get(),
		fdxFrameBuffer->NumViews, 
		uavNum,
		d3d11uavs,
		NULL
		);

	for (int i = 0; i < fdxFrameBuffer->NumViews; i++)
	{
		D3DFRI->DeviceContext->ClearRenderTargetView(fdxFrameBuffer->RenderTargetViews[i].Get(), &color.r);
	}

	if (fdxFrameBuffer->DepthStencilView.Get())
	{
		D3DFRI->DeviceContext->ClearDepthStencilView(fdxFrameBuffer->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

*/


void D3D11FRIDynamicAllocator::SetGeometrySource(FRIVertexBuffer* vertexBuffer, FRIIndexBuffer* indexBuffer, FRIInstanceBuffer* instanceBuffer)
{
	/*FD3D11VertexBuffer* VertexBuffer = vertexBuffer->As<FD3D11VertexBuffer>();
	
	if (instanceBuffer)
	{
		FD3D11InstanceBuffer* InstanceBuffer = instanceBuffer->As<FD3D11InstanceBuffer>();

		ID3D11Buffer* Buffers[2] = { VertexBuffer->Buffer.Get(), InstanceBuffer->Buffer.Get() };
		uint32		  Strides[2] = { VertexBuffer->LayoutStride, InstanceBuffer->LayoutStride };

		D3DFRI->DeviceContext->IASetInputLayout(VertexBuffer->InputLayout.Get());
		D3DFRI->DeviceContext->IASetVertexBuffers(0, 2, Buffers, Strides, ZEROOFFSET);
	}
	else
	{
		ID3D11Buffer* Buffers[1] = { VertexBuffer->Buffer.Get() };
		uint32		  Strides[1] = { VertexBuffer->LayoutStride };

		D3DFRI->DeviceContext->IASetInputLayout(VertexBuffer->InputLayout.Get());
		D3DFRI->DeviceContext->IASetVertexBuffers(0, 1, Buffers, Strides, ZEROOFFSET);
	}*/
}


void D3D11FRIDynamicAllocator::DrawPrimitives(EFRIPrimitiveType primitveType, uint32 vertexCount)
{
	if (primitveType == EFRIPrimitiveType::Lines)
		D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	else if (primitveType == EFRIPrimitiveType::Triangles)
		D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (primitveType == EFRIPrimitiveType::ControlPoint3)
		D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	else
		D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3DFRI->DeviceContext->Draw(vertexCount, 0);
}
void D3D11FRIDynamicAllocator::DrawPrimitivesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer)
{
	/*FD3D11IndexBuffer* IndexBuffer = indexBuffer->As<FD3D11IndexBuffer>();

	D3DFRI->DeviceContext->IASetIndexBuffer(IndexBuffer->Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	if (primitveType == EFRIPrimitiveType::Lines)
		D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	else if (primitveType == EFRIPrimitiveType::Triangles)
		D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (primitveType == EFRIPrimitiveType::ControlPoint3)
		D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	else
		D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	D3DFRI->DeviceContext->DrawIndexed(vertexCount, 0, 0);*/

}

void D3D11FRIDynamicAllocator::DrawInstances(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount)
{
	D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3DFRI->DeviceContext->DrawInstanced(vertexCount, instanceCount, 0, 0);
}
void D3D11FRIDynamicAllocator::DrawInstancesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType, FRIIndexBuffer* indexBuffer)
{
	/*FD3D11IndexBuffer* IndexBuffer = indexBuffer->As<FD3D11IndexBuffer>();

	D3DFRI->DeviceContext->IASetIndexBuffer(IndexBuffer->Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3DFRI->DeviceContext->DrawIndexedInstanced(vertexCount, instanceCount, 0, 0, 0);*/
}



void D3D11FRIDynamicAllocator::SetShaderPipeline(FRIShaderPipeline* shader)
{
	/*FD3D11ShaderPipeline* fdxShaderPipeline = shader->As<FD3D11ShaderPipeline>();

	D3DFRI->DeviceContext->VSSetShader(fdxShaderPipeline->vertexShader, 0, 0);
	D3DFRI->DeviceContext->GSSetShader(fdxShaderPipeline->geometryShader, 0, 0);
	D3DFRI->DeviceContext->HSSetShader(fdxShaderPipeline->hullShader, 0, 0);
	D3DFRI->DeviceContext->DSSetShader(fdxShaderPipeline->domainShader, 0, 0);
	D3DFRI->DeviceContext->PSSetShader(fdxShaderPipeline->pixelShader, 0, 0);
	D3DFRI->DeviceContext->CSSetShader(fdxShaderPipeline->computeShader, 0, 0);*/

}
void D3D11FRIDynamicAllocator::SetShaderConstantBuffer(uint32 slot, FRIConstantBuffer* uniformBuffer, uint32 attachFlags)
{
/*FD3D11ConstantBuffer* d3dubuff = uniformBuffer->As<FD3D11ConstantBuffer>();

	if (attachFlags & EFRI_Vertex)
		D3DFRI->DeviceContext->VSSetConstantBuffers(slot, 1, &(d3dubuff->Buffer));

	if (attachFlags & EFRI_Geometry)
		D3DFRI->DeviceContext->GSSetConstantBuffers(slot, 1, &(d3dubuff->Buffer));


	D3DFRI->DeviceContext->HSSetConstantBuffers(slot, 1, &(d3dubuff->Buffer));
	D3DFRI->DeviceContext->DSSetConstantBuffers(slot, 1, &(d3dubuff->Buffer));

	if (attachFlags & EFRI_Pixel)
		D3DFRI->DeviceContext->PSSetConstantBuffers(slot, 1, &(d3dubuff->Buffer));

	if (attachFlags & EFRI_Compute)
		D3DFRI->DeviceContext->CSSetConstantBuffers(slot, 1, &(d3dubuff->Buffer));*/

}
void D3D11FRIDynamicAllocator::SetShaderSamplerState(uint32 slot, FRISamplerState* samplerState)
{
	/*auto fdxSamplerState = samplerState->As<FD3D11SamplerState>()->Sampler.Get();

	D3DFRI->DeviceContext->VSSetSamplers(slot, 1, &fdxSamplerState);
	D3DFRI->DeviceContext->PSSetSamplers(slot, 1, &fdxSamplerState);
	D3DFRI->DeviceContext->DSSetSamplers(slot, 1, &fdxSamplerState);
	D3DFRI->DeviceContext->CSSetSamplers(slot, 1, &fdxSamplerState);*/
}

void D3D11FRIDynamicAllocator::SetShaderResource(uint32 slot, FRIShaderResourceView* srv)
{
	if (srv)
	{
		/*auto fdxSrv = srv->As<FD3D11ShaderResourceView>()->ShaderResourceView.Get();

		D3DFRI->DeviceContext->VSSetShaderResources(slot, 1, &fdxSrv);
		D3DFRI->DeviceContext->PSSetShaderResources(slot, 1, &fdxSrv);
		D3DFRI->DeviceContext->DSSetShaderResources(slot, 1, &fdxSrv);
		D3DFRI->DeviceContext->CSSetShaderResources(slot, 1, &fdxSrv);*/
	}
	else
	{
		D3DFRI->DeviceContext->VSSetShaderResources(slot, 1, FRID3DEmptySRV);
		D3DFRI->DeviceContext->PSSetShaderResources(slot, 1, FRID3DEmptySRV);
		D3DFRI->DeviceContext->DSSetShaderResources(slot, 1, FRID3DEmptySRV);
		D3DFRI->DeviceContext->CSSetShaderResources(slot, 1, FRID3DEmptySRV);
	}
}

void D3D11FRIDynamicAllocator::BeginScene()
{

}
void D3D11FRIDynamicAllocator::BeginFrame()
{
	//D3DFRI->DeviceContext->RSSetViewports(1, &D3DFRI->Viewport);
}
void D3D11FRIDynamicAllocator::EndScene()

{

}
void D3D11FRIDynamicAllocator::EndFrame()
{
	D3DFRI->DeviceContext->PSSetShaderResources(0, 16, FRID3DEmptySRV);
	D3DFRI->DeviceContext->GSSetShaderResources(0, 16, FRID3DEmptySRV);
	D3DFRI->DeviceContext->CSSetShaderResources(0, 16, FRID3DEmptySRV);
	D3DFRI->DeviceContext->VSSetShaderResources(0, 16, FRID3DEmptySRV);
}




void D3D11FRIDynamicAllocator::FlushMipMaps(FRIShaderResourceView* resource)
{
	/*auto fdxResource = resource->As<FD3D11ShaderResourceView>();
	D3DFRI->DeviceContext->GenerateMips(fdxResource->ShaderResourceView.Get());*/
}


void D3D11FRIDynamicAllocator::ResourceSubdata(FRIResourceObject* resource, FRIUpdateDescriptor data)
{
	auto d3dResource = static_cast<FD3DResource*>(resource->GetResource())->D3DResource();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DFRI->DeviceContext->Map(d3dResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Memory::Copy(mappedResource.pData, data.DataArray, data.ByteSize);
	D3DFRI->DeviceContext->Unmap(d3dResource, 0);
}


void D3D11FRIDynamicAllocator::StageResources(FRIConstantBuffer* uniformBuffer, FRIMemoryStageDelegate stageLambda)
{

/*	FD3D11ConstantBuffer* fdxConstantBuffer = uniformBuffer->As<FD3D11ConstantBuffer>();
	FRIMemoryMap memory;
	memory.Head = 0;
	memory.MemoryPtr = (byte*)_aligned_malloc(fdxConstantBuffer->ByteSize, 16);

	stageLambda(memory);


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DFRI->DeviceContext->Map(fdxConstantBuffer->Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	{
		memcpy(mappedResource.pData, memory.MemoryPtr, fdxConstantBuffer->ByteSize);
	}
	D3DFRI->DeviceContext->Unmap(fdxConstantBuffer->Buffer.Get(), 0);

	_aligned_free(memory.MemoryPtr);*/
}

void D3D11FRIDynamicAllocator::SetRasterizerState(FRIRasterizerState* rasterizer)
{
	//D3DFRI->DeviceContext->RSSetState(rasterizer->As<FD3D11RasterizerState>()->RasterizerState.Get());
}
void D3D11FRIDynamicAllocator::SetBlendState(FRIBlendState* blend)
{
	//D3DFRI->DeviceContext->OMSetBlendState(blend->As<FD3D11BlendState>()->BlendState.Get(), 0, 0xffffffff);
}
void D3D11FRIDynamicAllocator::SetDepthStencilState(FRIDepthStencilState* depth)
{
	//D3DFRI->DeviceContext->OMSetDepthStencilState(depth->As<FD3D11DepthStencilState>()->DepthStencilState.Get(), 1);
}

void D3D11FRIDynamicAllocator::CopyResource(FRIResourceObject* source, FRIResourceObject* dest)
{
	auto fdxSource = static_cast<FD3DResource*>(source->GetResource())->D3DResource();
	auto fdxDest = static_cast<FD3DResource*>(dest->GetResource())->D3DResource();

	D3DFRI->DeviceContext->CopyResource(fdxDest, fdxSource);
}


void D3D11FRIDynamicAllocator::SetUnorderedAccessView(uint32 slot, FRIUnorderedAccessView* source)
{
	if (source)
	{
		/*auto fdxUav = source->As<FD3D11UnorderedAccessView>();
		D3DFRI->DeviceContext->CSSetUnorderedAccessViews(slot, 1, &fdxUav->UnorderedAccessView, NULL);*/
	}
	else
	{
		D3DFRI->DeviceContext->CSSetUnorderedAccessViews(slot, 1, FRID3DEmptyUAV, NULL);
	}
}

void D3D11FRIDynamicAllocator::DispatchCompute(uint32 x, uint32 y, uint32 z)
{
	D3DFRI->DeviceContext->Dispatch(x, y, z);
}

FRIPipelineStateObject* D3D11FRIDynamicAllocator::CreatePipelineStateObject(
	FRIInputLayout inputLayout,
	FArray<FRIRootParameter> RootParameters,
	FArray<FRIStaticSampler> StaticSamplers,
	const ShaderLibraryModule& shaderModule,
	FArray<EFRITextureFormat> RTVFormats,
	uint32 NumRenderTargets
)
{
	return NULL;
}
