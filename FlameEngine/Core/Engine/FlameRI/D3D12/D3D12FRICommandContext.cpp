#include "D3D12FRIContext.h"
#include "D3D12FRICommon.h"


D3D12FRICommandContext::D3D12FRICommandContext(FD3D12Device* Parent) : 
	FD3D12DeviceChild(Parent),
	FrameCache(Parent),
	BackBufferIndex(0)
{
	auto Device = Parent->GetDevice();
    RTVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (int i = 0; i < Parent->BackBufferCount; i++)
	{
		// Create Command Allocator
		CommandAllocators.Add(NULL);
		ThrowIfFailed(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocators[i])));

	}
	ThrowIfFailed(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocators[0], nullptr, IID_PPV_ARGS(CommandList.GetAddressOf())));
	ThrowIfFailed(CommandList->Close());

	for (int i = 0; i < Parent->BackBufferCount; i++)
	{
		auto BackBufferRT = new FD3D12Resource(Parent);
		BackBufferRT->CurrentState = D3D12_RESOURCE_STATE_PRESENT;
		RenderTargets.Add(BackBufferRT);
		Parent->SwapChain->GetBuffer(i, IID_PPV_ARGS(RenderTargets.Last()->Resource.GetAddressOf()));

		D3D12_RENDER_TARGET_VIEW_DESC Desc;
		ZeroMem(Desc);
		Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		BoundRenderTargets.Add(new FD3D12RenderTargetView(Parent, Desc, RenderTargets.Last()));

		Fences.Add(NULL);
		FenceValues.Add(0); // set the initial fence value to 0
		ThrowIfFailed(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fences[i])));
	}

	// create a handle to a fence event
	FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	DSV = new FD3D12Resource(
		L"Frame DSV",
		Parent,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, Parent->Context->GetViewport().Width, Parent->Context->GetViewport().Height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	BoundDSV = new FD3D12DepthStencilView(Parent, depthStencilDesc, DSV);

}

D3D12_VIEWPORT Viewport = {};
D3D12_RECT Scissor = {};

void D3D12FRICommandContext::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
{
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = width;
	Viewport.Height = height;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	Scissor.left = 0;
	Scissor.top = 0;
	Scissor.right = width;
	Scissor.bottom = height;

	CommandList->RSSetViewports(1, &Viewport);
	CommandList->RSSetScissorRects(1, &Scissor);
}
void D3D12FRICommandContext::BindRenderTargets(uint32 Count, FRIRenderTargetView** renderTargets, FRIDepthStencilView* DSV)
{
	auto Device = GetParentDevice();
	if (Count == 0 || renderTargets == NULL)
	{
		auto RTVIndex = Device->SwapChain->GetCurrentBackBufferIndex();
		auto RTView = BoundRenderTargets[RTVIndex];

		if (RTView)
		{
			auto handle = RTView->GetView();
			auto dsvHandle = BoundDSV->GetView();
			BoundDSV->ResourceHandle->TransitionState(CommandList.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
			CommandList->OMSetRenderTargets(1, &handle, FALSE, &dsvHandle);
		}
	}
	else
	{
		auto handles = new D3D12_CPU_DESCRIPTOR_HANDLE[Count];
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
		for (uint32 RTVIndex = 0; RTVIndex < Count; RTVIndex++)
		{
			auto RTV = DX12::Cast<FD3D12RenderTargetView>(renderTargets[RTVIndex]);
			auto RT = RTV->ResourceHandle;
			RT->TransitionState(CommandList.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET);
			handles[RTVIndex] = RTV->GetView();
		}

		if (DSV)
		{
			auto FRIDSV = DX12::Cast<FD3D12DepthStencilView>(DSV);
			dsvHandle = FRIDSV->GetView();
			FRIDSV->ResourceHandle->TransitionState(CommandList.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
		}

		CommandList->OMSetRenderTargets(Count, handles, FALSE, DSV ? &dsvHandle : NULL);
		//delete[] handles;
	}
}


void D3D12FRICommandContext::ClearRenderTarget(FRIRenderTargetView* buffer, Color32 color, FRIUnorderedAccessView** uavs, uint32 uavNum)
{
	auto Device = GetParentDevice();
	FD3D12RenderTargetView* RTView = NULL;

	if (!buffer)
	{
		auto RTVIndex = Device->SwapChain->GetCurrentBackBufferIndex();
		RTView = BoundRenderTargets[RTVIndex];
	}
	else
	{
		RTView = DX12::Cast<FD3D12RenderTargetView>(buffer);
	}
	
	auto handle = RTView->GetView();
	CommandList->ClearRenderTargetView(handle, &color.r, 0, NULL);
}

void D3D12FRICommandContext::ClearDepthStencil(FRIDepthStencilView* dsv, float depth)
{
	auto DSView = !dsv ? BoundDSV : DX12::Cast<FD3D12DepthStencilView>(dsv);
	auto handle = DSView->GetView();
	CommandList->ClearDepthStencilView(handle, D3D12_CLEAR_FLAG_DEPTH, depth, 0, 0, NULL);
}

void D3D12FRICommandContext::SetGeometrySource(FRIVertexBuffer* vertexBuffer, FRIIndexBuffer* indexBuffer, FRIInstanceBuffer* instanceBuffer)
{
	FD3D12VertexBuffer* VertexBuffer = DX12::Cast<FD3D12VertexBuffer>(vertexBuffer);
	FD3D12IndexBuffer* IndexBuffer = DX12::Cast<FD3D12IndexBuffer>(indexBuffer);
	auto Device = GetParentDevice();

	if (instanceBuffer)
	{
		/*FD3D12InstanceBuffer* InstanceBuffer = instanceBuffer->As<FD3D12InstanceBuffer>();

		ID3D12Buffer* Buffers[2] = { VertexBuffer->Buffer.Get(), InstanceBuffer->Buffer.Get() };
		uint32		  Strides[2] = { VertexBuffer->LayoutStride, InstanceBuffer->LayoutStride };

		D3DFRI->DeviceContext->IASetInputLayout(VertexBuffer->InputLayout.Get());
		D3DFRI->DeviceContext->IASetVertexBuffers(0, 2, Buffers, Strides, ZEROOFFSET);*/
	}
	else
	{
		D3D12_VERTEX_BUFFER_VIEW* pVertexBuffers = new D3D12_VERTEX_BUFFER_VIEW[1]{ VertexBuffer->GetView() };
		CommandList->IASetVertexBuffers(0, 1, pVertexBuffers);
		CommandList->IASetIndexBuffer(&IndexBuffer->GetView());
	}
	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void D3D12FRICommandContext::DrawPrimitives(EFRIPrimitiveType primitveType, uint32 vertexCount)
{
	FrameCache.ApplyState(CommandList.Get());
	CommandList->DrawInstanced(vertexCount, 1, 0, 0);
}
void D3D12FRICommandContext::DrawPrimitivesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, EFRIIndexType indexType)
{
	FrameCache.ApplyState(CommandList.Get());
	CommandList->DrawIndexedInstanced(vertexCount, 1, 0, 0, 0);
}

void D3D12FRICommandContext::DrawInstances(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount)
{
	FrameCache.ApplyState(CommandList.Get());
	CommandList->DrawInstanced(vertexCount, instanceCount, 0, 0);
}
void D3D12FRICommandContext::DrawInstancesIndexed(EFRIPrimitiveType primitveType, uint32 vertexCount, uint32 instanceCount, EFRIIndexType indexType)
{
	FrameCache.ApplyState(CommandList.Get());
	CommandList->DrawIndexedInstanced(vertexCount, instanceCount, 0, 0, 0);
}
void D3D12FRICommandContext::DispatchCompute(uint32 x, uint32 y, uint32 z)
{
	FrameCache.ApplyComputeState(CommandList.Get());
	CommandList->Dispatch(x, y, z);
}


void D3D12FRICommandContext::WaitForPreviousFrame()
{
	auto Device = GetParentDevice();
	// swap the current rtv buffer index so we draw on the correct buffer
	auto frameIndex = Device->SwapChain->GetCurrentBackBufferIndex();

	// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
	// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
	if (Fences[frameIndex]->GetCompletedValue() < FenceValues[frameIndex])
	{
		// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
		auto hr = Fences[frameIndex]->SetEventOnCompletion(FenceValues[frameIndex], FenceEvent);
		if (FAILED(hr))
		{
			//Running = false;
		}

		// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
		// has reached "fenceValue", we know the command queue has finished executing
		WaitForSingleObject(FenceEvent, INFINITE);
	}

	// increment fenceValue for next frame
	FenceValues[frameIndex]++;
}

void D3D12FRICommandContext::BeginFrame()
{

	WaitForPreviousFrame();

	// Transition current backbuffer to rendering state
	auto Device = GetParentDevice();
	auto RTVIndex = Device->SwapChain->GetCurrentBackBufferIndex();
	auto RTView = BoundRenderTargets[RTVIndex];

	// Reset Command List
	ThrowIfFailed(CommandAllocators[RTVIndex]->Reset());
	ThrowIfFailed(CommandList->Reset(CommandAllocators[RTVIndex], nullptr));

	if (RTView)
	{
		RenderTargets[RTVIndex]->TransitionState(CommandList.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET);
	}

	FrameCache.InitFrame(CommandList.Get());
}

void D3D12FRICommandContext::EndFrame()
{
	// Transition Current Backbuffer to present state
	auto Device = GetParentDevice();
	auto RTVIndex = Device->SwapChain->GetCurrentBackBufferIndex();
	auto RTView = BoundRenderTargets[RTVIndex];

	if (RTView)
	{
		RenderTargets[RTVIndex]->TransitionState(CommandList.Get(), D3D12_RESOURCE_STATE_PRESENT);
	}


	// Close and Execute CommandList
	ThrowIfFailed(CommandList->Close());
	GetParentDevice()->CommandQueue->ExecuteCommandLists(1, CommandListCast(CommandList.GetAddressOf()));
	ThrowIfFailed(GetParentDevice()->CommandQueue->Signal(Fences[RTVIndex], FenceValues[RTVIndex]));
}


void D3D12FRICommandContext::BeginCompute()
{
	FrameCache.InitFrame(CommandList.Get());
}
void D3D12FRICommandContext::EndCompute()
{

}

void D3D12FRICommandContext::OpenCommandList()
{
	ThrowIfFailed(CommandList->Reset(CommandAllocators[0], nullptr));
}


void D3D12FRICommandContext::CloseCommandList()
{
	ThrowIfFailed(CommandList->Close());
	GetParentDevice()->CommandQueue->ExecuteCommandLists(1, CommandListCast(CommandList.GetAddressOf()));
}

void D3D12FRICommandContext::GenerateMipMaps(FRIShaderResourceView* resource)
{

}


void D3D12FRICommandContext::ResourceSubdata(FRIResourceObject* resource, FRIUpdateDescriptor data)
{
	auto Desc = D3DConvertDescriptorToSubresourceBuffer(1, data);
	auto Buffer = DX12::DynCast<FD3D12TransientResource>(resource);

	Buffer->TargetResource->TransitionState(CommandList.Get(), D3D12_RESOURCE_STATE_COPY_DEST);
	Buffer->UpdateSubresources(CommandList.Get(), 1, &Desc);
	Buffer->TargetResource->TransitionState(CommandList.Get(), Buffer->WorkingState);
}



void D3D12FRICommandContext::CopyResource(FRIResourceObject* source, FRIResourceObject* dest)
{

}


void D3D12FRICommandContext::SetPipelineState(FRIPipelineStateObject* pipelineState)
{
	FrameCache.SetPSO(DX12::Cast<FD3D12PipelineStateObject>(pipelineState));
}

/*
*	Set SRVs
*/

void D3D12FRICommandContext::SetShaderResourceViewVS(uint32 Slot, FRIShaderResourceView* View)
{
	FrameCache.SetShaderResourceView<SF_Vertex>(Slot, View);
}

void D3D12FRICommandContext::SetShaderResourceViewPS(uint32 Slot, FRIShaderResourceView* View)
{
	FrameCache.SetShaderResourceView<SF_Pixel>(Slot, View);
}

void D3D12FRICommandContext::SetShaderResourceViewHS(uint32 Slot, FRIShaderResourceView* View)
{
	FrameCache.SetShaderResourceView<SF_Hull>(Slot, View);
}

void D3D12FRICommandContext::SetShaderResourceViewDS(uint32 Slot, FRIShaderResourceView* View)
{
	FrameCache.SetShaderResourceView<SF_Domain>(Slot, View);
}

void D3D12FRICommandContext::SetShaderResourceViewGS(uint32 Slot, FRIShaderResourceView* View)
{
	FrameCache.SetShaderResourceView<SF_Geometry>(Slot, View);
}

void D3D12FRICommandContext::SetShaderResourceViewCS(uint32 Slot, FRIShaderResourceView* View)
{
	FrameCache.SetShaderResourceView<SF_Compute>(Slot, View);
}

/*
*	Set CBVs
*/

void D3D12FRICommandContext::SetConstantBufferViewVS(uint32 Slot, FRIConstantBufferView* View)
{
	FrameCache.SetConstantBufferView<SF_Vertex>(Slot, View);
}

void D3D12FRICommandContext::SetConstantBufferViewPS(uint32 Slot, FRIConstantBufferView* View)
{
	FrameCache.SetConstantBufferView<SF_Pixel>(Slot, View);
}

void D3D12FRICommandContext::SetConstantBufferViewHS(uint32 Slot, FRIConstantBufferView* View)
{
	FrameCache.SetConstantBufferView<SF_Hull>(Slot, View);
}

void D3D12FRICommandContext::SetConstantBufferViewDS(uint32 Slot, FRIConstantBufferView* View)
{
	FrameCache.SetConstantBufferView<SF_Domain>(Slot, View);
}

void D3D12FRICommandContext::SetConstantBufferViewGS(uint32 Slot, FRIConstantBufferView* View)
{
	FrameCache.SetConstantBufferView<SF_Geometry>(Slot, View);
}

void D3D12FRICommandContext::SetConstantBufferViewCS(uint32 Slot, FRIConstantBufferView* View)
{
	FrameCache.SetConstantBufferView<SF_Compute>(Slot, View);
}

/*
*   Set UAvs
*/

void D3D12FRICommandContext::SetUnorderedAccessViewPS(uint32 Slot, FRIUnorderedAccessView* View)
{
	FrameCache.SetUnorderedAccessViewPS(Slot, View);
}

void D3D12FRICommandContext::SetUnorderedAccessViewCS(uint32 Slot, FRIUnorderedAccessView* View)
{
	FrameCache.SetUnorderedAccessViewCS(Slot, View);
}

/* Set Samplers */



void D3D12FRICommandContext::SetShaderSamplerVS(uint32 Slot, FRISamplerState* Sampler)
{
	FrameCache.SetSampler<SF_Vertex>(Slot, Sampler);
}
void D3D12FRICommandContext::SetShaderSamplerPS(uint32 Slot, FRISamplerState* Sampler)
{
	FrameCache.SetSampler<SF_Pixel>(Slot, Sampler);
}
void D3D12FRICommandContext::SetShaderSamplerHS(uint32 Slot, FRISamplerState* Sampler)
{
	FrameCache.SetSampler<SF_Hull>(Slot, Sampler);
}
void D3D12FRICommandContext::SetShaderSamplerDS(uint32 Slot, FRISamplerState* Sampler)
{
	FrameCache.SetSampler<SF_Domain>(Slot, Sampler);
}
void D3D12FRICommandContext::SetShaderSamplerGS(uint32 Slot, FRISamplerState* Sampler)
{
	FrameCache.SetSampler<SF_Geometry>(Slot, Sampler);
}
void D3D12FRICommandContext::SetShaderSamplerCS(uint32 Slot, FRISamplerState* Sampler)
{
	FrameCache.SetSampler<SF_Vertex>(Slot, Sampler);

}

