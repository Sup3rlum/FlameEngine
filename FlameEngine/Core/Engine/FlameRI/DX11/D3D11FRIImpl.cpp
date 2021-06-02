#include "D3D11FRIDynamic.h"
#include "D3D11FRIContext.h"
#include "D3D11FRIResource.h"

#include "Core/Framework/IO/FileStream.h"


const uint32 ZEROOFFSET[] = { 0 };


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

ID3D11RenderTargetView* FRID3DEmptyRTV[8] = { 0 };
ID3D11ShaderResourceView* FRID3DEmptySRV[8] = { 0 };


FRIIndexBuffer* D3D11FRIDynamicAllocator::CreateIndexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor)
{
	return new FD3D11IndexBuffer(D3DFRI->Device, Size, Usage, resourceDescriptor);

}
FRIVertexBuffer* D3D11FRIDynamicAllocator::CreateVertexBuffer(uint32 Size, uint32 Usage, FRICreationDescriptor resourceDescriptor)
{
	return new FD3D11VertexBuffer(D3DFRI->Device, Size, Usage, resourceDescriptor);
}



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
	return NULL;
}
FRIHullShader* D3D11FRIDynamicAllocator::CreateHullShader(const FArray<byte>& binCode)
{
	return NULL;
}
FRIDomainShader* D3D11FRIDynamicAllocator::CreateDomainShader(const FArray<byte>& binCode)
{
	return NULL;
}
FRIComputeShader* D3D11FRIDynamicAllocator::CreateComputeShader(const FArray<byte>& binCode)
{
	return NULL;
}



FRIShaderPipeline* D3D11FRIDynamicAllocator::CreateShaderPipeline(FRIShaderPipelineCreationDescriptor descriptor)
{
	return NULL;
}

FRIShaderPipeline* D3D11FRIDynamicAllocator::CreateShaderPipeline(const ShaderLibraryModule& shaderModule)
{
	FArray<FRIShaderBase*> shaderArray;
	FArray<EFRIResourceShaderType> typesArray;

	for (const auto& part : shaderModule.Parts)
	{
		FRIShaderBase* shaderResource = NULL;

		switch (part.Key)
		{
		case EFRIResourceShaderType::Vertex: shaderResource = CreateVertexShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Pixel: shaderResource = CreatePixelShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Geometry: shaderResource = CreateGeometryShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Hull: shaderResource = CreateHullShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Domain: shaderResource = CreateDomainShader(part.Value.Memory); break;
		case EFRIResourceShaderType::Compute: shaderResource = CreateComputeShader(part.Value.Memory); break;

		}

		shaderArray.Add(shaderResource);
		typesArray.Add(part.Key);
	}

	return new FD3D11ShaderPipeline(FRIShaderPipelineCreationDescriptor(shaderArray.Length(), shaderArray.Begin()), typesArray);
}

FRITexture2D* D3D11FRIDynamicAllocator::CreateTexture2D(uint32 width, uint32 height, uint32 sampleCount, EFRITextureFormat format, FRIColorDataFormat colorData, FRICreationDescriptor resourceDescriptor)
{
	DXGI_FORMAT gpuformat = EDX11FormatProxyEnum(format);
	return new FD3D11Texture2D(D3DFRI->Device, width, height, sampleCount, gpuformat, resourceDescriptor);
}
FRITexture3D* D3D11FRIDynamicAllocator::CreateTexture3D(uint32 width, uint32 height, uint32 depth, FRICreationDescriptor resourceDescriptor)
{
	return NULL;
}


FRITexture2DArray* D3D11FRIDynamicAllocator::CreateTexture2DArray(uint32 width, uint32 height, uint32 numLayers, EFRITextureFormat format, FRIColorDataFormat colorData, FRICreationDescriptor resourceDescriptor)
{
	DXGI_FORMAT gpuformat = EDX11FormatProxyEnum(format);
	return new FD3D11Texture2DArray(D3DFRI->Device, width, height, numLayers, gpuformat);
}

FRIUniformBuffer* D3D11FRIDynamicAllocator::CreateUniformBuffer(FRICreationDescriptor resourceDescriptor)
{

	return new FD3D11UniformBuffer(D3DFRI->Device, resourceDescriptor);
}


FRIFrameBuffer* D3D11FRIDynamicAllocator::CreateFrameBuffer(FArray<FRIFrameBufferAttachment> textureAttachments, bool enableDepthRenderBuffer)
{
	return new FD3D11FrameBuffer(D3DFRI->Device, textureAttachments, enableDepthRenderBuffer);
}
FRIFrameBuffer* D3D11FRIDynamicAllocator::CreateFrameBuffer(FRIFrameBufferArrayAttachment textureAttachment, bool enableDepthRenderBuffer)
{
	return new FD3D11FrameBuffer(D3DFRI->Device, textureAttachment, enableDepthRenderBuffer);
}



FRIVertexDeclaration* D3D11FRIDynamicAllocator::CreateVertexDeclaration(FArray<FRIVertexDeclarationComponent> DeclCompArray, FRIVertexShader* signatureShader)
{
	return new FD3D11VertexDeclaration(D3DFRI->Device, DeclCompArray, static_cast<FD3D11VertexShader*>(signatureShader));
}

void D3D11FRIDynamicAllocator::AttachVertexDeclaration(FRIVertexBuffer* geometry, FRIVertexDeclaration* declaration)
{
	auto fdxVertexDeclaration = static_cast<FD3D11VertexDeclaration*>(declaration);
	auto fdxGeometry = static_cast<FD3D11VertexBuffer*>(geometry);

	fdxGeometry->InputLayout = fdxVertexDeclaration->InputLayout;
	fdxGeometry->LayoutStride = fdxVertexDeclaration->LayoutStride;
}

FRIRasterizerState* D3D11FRIDynamicAllocator::CreateRasterizerState(EFRICullMode cullMode, EFRIFillMode fillmode)
{
	return new FD3D11RasterizerState(D3DFRI->Device, FRID3DCullModeEnums[(uint32)cullMode], FRID3DFillModeEnums[(uint32)fillmode]);
}

FRIBlendState* D3D11FRIDynamicAllocator::CreateBlendState(EFRIAlphaBlend srcBlend, EFRIAlphaBlend dstBlend)
{
	return new FD3D11BlendState(D3DFRI->Device, FRID3DBlendEnums[(uint32)srcBlend], FRID3DBlendEnums[(uint32)dstBlend]);
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
void D3D11FRIDynamicAllocator::BindFrameBuffer(FRIFrameBuffer* frameBuffer)
{
	FD3D11FrameBuffer* fdxFrameBuffer = static_cast<FD3D11FrameBuffer*>(frameBuffer);

	D3DFRI->DeviceContext->OMSetRenderTargets(
		fdxFrameBuffer->NumViews,
		&fdxFrameBuffer->RenderTargetViews[0],
		fdxFrameBuffer->DepthStencilView
	);
}
void D3D11FRIDynamicAllocator::UnbindFrameBuffer()
{
	D3DFRI->DeviceContext->OMSetRenderTargets(
		8,
		FRID3DEmptyRTV,
		NULL
	);

	D3DFRI->DeviceContext->OMSetRenderTargets(1, &D3DFRI->RenderTargetView, D3DFRI->DepthStencilView);
}

void D3D11FRIDynamicAllocator::ClearBuffer(FRIFrameBuffer* buffer, Color color)
{

	if (buffer)
	{
		BindFrameBuffer(buffer);
		FD3D11FrameBuffer* fdxFrameBuffer = static_cast<FD3D11FrameBuffer*>(buffer);

		for (int i = 0; i < fdxFrameBuffer->NumViews; i++)
		{
			D3DFRI->DeviceContext->ClearRenderTargetView(fdxFrameBuffer->RenderTargetViews[i], &color[0]);
		}

		if (fdxFrameBuffer->DepthStencilView)
		{
			D3DFRI->DeviceContext->ClearDepthStencilView(fdxFrameBuffer->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}
	else
	{
		D3DFRI->DeviceContext->OMSetRenderTargets(1, &D3DFRI->RenderTargetView, D3DFRI->DepthStencilView);

		D3DFRI->DeviceContext->ClearRenderTargetView(D3DFRI->RenderTargetView, &color[0]);
		D3DFRI->DeviceContext->ClearDepthStencilView(D3DFRI->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}


}



void D3D11FRIDynamicAllocator::SetGeometrySource(FRIVertexBuffer* vertexBuffer)
{
	FD3D11VertexBuffer* VertexBuffer = static_cast<FD3D11VertexBuffer*>(vertexBuffer);

	D3DFRI->DeviceContext->IASetInputLayout(VertexBuffer->InputLayout);
	D3DFRI->DeviceContext->IASetVertexBuffers(0, 1, &(VertexBuffer->Buffer), &(VertexBuffer->LayoutStride), ZEROOFFSET);

}
void D3D11FRIDynamicAllocator::DrawPrimitives(uint32 elementType, uint32 elementCount)
{
	D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3DFRI->DeviceContext->Draw(elementCount, 0);
}
void D3D11FRIDynamicAllocator::DrawPrimitivesIndexed(uint32 elementType, uint32 elementCount, uint32 indexType, FRIIndexBuffer* indexBuffer)
{
	FD3D11IndexBuffer* IndexBuffer = static_cast<FD3D11IndexBuffer*>(indexBuffer);

	D3DFRI->DeviceContext->IASetIndexBuffer(IndexBuffer->Buffer, DXGI_FORMAT_R32_UINT, 0);
	D3DFRI->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DFRI->DeviceContext->DrawIndexed(IndexBuffer->IndexCount, 0, 0);

}

void D3D11FRIDynamicAllocator::SetShaderPipeline(FRIShaderPipeline* shader)
{
	FD3D11ShaderPipeline* fdxShaderPipeline = static_cast<FD3D11ShaderPipeline*>(shader);
	if (fdxShaderPipeline->vertexShader)
	{
		D3DFRI->DeviceContext->VSSetShader(fdxShaderPipeline->vertexShader, 0, 0);
	}
	if (fdxShaderPipeline->pixelShader)
	{
		D3DFRI->DeviceContext->PSSetShader(fdxShaderPipeline->pixelShader, 0, 0);
	}
}
void D3D11FRIDynamicAllocator::SetShaderUniformBuffer(uint32 slot, FRIUniformBuffer* uniformBuffer)
{
	FD3D11UniformBuffer* d3dubuff = static_cast<FD3D11UniformBuffer*>(uniformBuffer);

	D3DFRI->DeviceContext->VSSetConstantBuffers(slot, 1, &(d3dubuff->Buffer));
	D3DFRI->DeviceContext->PSSetConstantBuffers(slot, 1, &(d3dubuff->Buffer));

}
void D3D11FRIDynamicAllocator::SetShaderSampler(FUniformSampler* sampler)
{
	ID3D11ShaderResourceView* srv = NULL;
	ID3D11SamplerState* samplerState = NULL;

	switch (sampler->samplerType)
	{
	case EFRIUniformSamplerType::TSampler2D:
	{
		FD3D11Texture2D* fdxTex = static_cast<FD3D11Texture2D*>(sampler->Param2D);
		srv = fdxTex->ShaderResourceView;
		samplerState = fdxTex->Sampler;
		break;
	}
	case EFRIUniformSamplerType::TSampler2DArray:
	{
		FD3D11Texture2DArray* fdxTex = static_cast<FD3D11Texture2DArray*>(sampler->Param2DArray);
		srv = fdxTex->ShaderResourceView;
		samplerState = fdxTex->Sampler;
		break;
	}
	default:
		break;
	}

	D3DFRI->DeviceContext->PSSetShaderResources(sampler->Unit, 1, &srv);
	D3DFRI->DeviceContext->PSSetSamplers(sampler->Unit, 1, &samplerState);
}
void D3D11FRIDynamicAllocator::SetTextureParameterBuffer(FRITexture2D* texture, FRITextureParameterBuffer param)
{

}
void D3D11FRIDynamicAllocator::SetTextureParameterBuffer(FRITexture2DArray* texture, FRITextureParameterBuffer param)
{

}

void D3D11FRIDynamicAllocator::SetFramebufferTextureLayer(FRIFrameBuffer* tex, uint32 layer)
{
	auto fdxFrameBuffer = static_cast<FD3D11FrameBuffer*>(tex);

	D3DFRI->DeviceContext->OMSetRenderTargets(1, &fdxFrameBuffer->RenderTargetViews[layer], fdxFrameBuffer->DepthStencilView);
	D3DFRI->DeviceContext->ClearDepthStencilView(fdxFrameBuffer->DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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
	D3DFRI->DeviceContext->PSSetShaderResources(0, 8, FRID3DEmptySRV);
}



void D3D11FRIDynamicAllocator::SetBackCull(bool back)

{

}

void D3D11FRIDynamicAllocator::FlushMipMaps(FRITexture2D* tex)
{

}
void D3D11FRIDynamicAllocator::FlushMipMaps(FRITexture2DArray* tex)

{

}


void D3D11FRIDynamicAllocator::UniformBufferSubdata(FRIUniformBuffer* uniformBuffer, FRIUpdateDescriptor resource)
{
	FD3D11UniformBuffer* fdxUniformBuffer = static_cast<FD3D11UniformBuffer*>(uniformBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	D3DFRI->DeviceContext->Map(fdxUniformBuffer->Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, resource.DataArray, resource.ByteSize);
	D3DFRI->DeviceContext->Unmap(fdxUniformBuffer->Buffer, 0);

}

void D3D11FRIDynamicAllocator::StageResources(FRIResourceStageLambda stageLambda)
{
	stageLambda();
}

void D3D11FRIDynamicAllocator::SetRasterizerState(FRIRasterizerState* rasterizer)
{
	D3DFRI->DeviceContext->RSSetState(static_cast<FD3D11RasterizerState*>(rasterizer)->RasterizerState);
}
void D3D11FRIDynamicAllocator::SetBlendState(FRIBlendState* blend)
{
	D3DFRI->DeviceContext->OMSetBlendState(static_cast<FD3D11BlendState*>(blend)->BlendState, 0, 0xffffffff);
}

