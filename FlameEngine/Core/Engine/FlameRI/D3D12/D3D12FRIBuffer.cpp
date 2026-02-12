#include "D3D12FRICommon.h"
#include "D3D12FRIContext.h"
#include "Core/Common/Alignment.h"

D3D12_VERTEX_BUFFER_VIEW CreateVertexBufferView(ID3D12Resource* BufferResource, uint32 Stride, uint32 Size)
{
	D3D12_VERTEX_BUFFER_VIEW View;
	View.BufferLocation = BufferResource->GetGPUVirtualAddress();
	View.StrideInBytes = Stride;
	View.SizeInBytes = Size * Stride;

	return View;
}

D3D12_INDEX_BUFFER_VIEW CreateIndexBufferView(ID3D12Resource* BufferResource, uint32 Stride, uint32 Size)
{
	D3D12_INDEX_BUFFER_VIEW View;
	View.BufferLocation = BufferResource->GetGPUVirtualAddress();
	View.Format = DXGI_FORMAT_R32_UINT;
	View.SizeInBytes = Size * sizeof(int);

	return View;
}

void FD3D12Buffer::CreateBuffer(FRICreationDescriptor dataDesc, uint32 ByteSize, uint64 Alignment, bool bAllowUAV, D3D12_RESOURCE_STATES workingState)
{
	WorkingState = workingState;
	auto Parent = GetParentDevice();
	auto Device = Parent->GetDevice();

	const auto UploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto DefaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	const auto TargetResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(Align(ByteSize, Alignment), bAllowUAV ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : D3D12_RESOURCE_FLAG_NONE);
	const auto UploadResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(Align(ByteSize, Alignment));

	TargetResource = new FD3D12Resource(
		"Unnamed Buffer",
		Parent,
		DefaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		TargetResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr
	);

	IntermediateResource = new FD3D12Resource(
		"Unnamed Buffer (Upload)",
		Parent,
		UploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		UploadResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr
	);

	if (!!dataDesc.DataArray)
	{
		assert(dataDesc.ByteSize == ByteSize);
		D3D12_SUBRESOURCE_DATA BufferData = {};
		BufferData.pData = reinterpret_cast<BYTE*>(dataDesc.DataArray);
		BufferData.RowPitch = dataDesc.ByteSize;
		BufferData.SlicePitch = 0;

		UpdateSubresources(1, &BufferData);
	}
	TargetResource->TransitionState(D3D12_RESOURCE_STATE_COPY_DEST, workingState);
}

FD3D12VertexBuffer::FD3D12VertexBuffer(FD3D12Device* Parent, uint32 Size, uint32 Stride, FRICreationDescriptor resourceData) :
	FD3D12Buffer(Parent),
	FRIVertexBuffer(Size, Stride, EFRIUsage::Default, EFRIAccess::None)
{
	CreateBuffer(resourceData, Size * Stride, 4);
	View = CreateVertexBufferView(TargetResource->Resource.Get(), Stride, Size);
}


FD3D12IndexBuffer::FD3D12IndexBuffer(FD3D12Device* Parent, uint32 Size, uint32 Stride, FRICreationDescriptor resourceData) :
	FD3D12Buffer(Parent),
	FRIIndexBuffer(Size, EFRIUsage::Default, EFRIAccess::None)
{
	CreateBuffer(resourceData, Size * Stride, 4);
	View = CreateIndexBufferView(TargetResource->Resource.Get(), Stride, Size);
}

FD3D12ComputeBuffer::FD3D12ComputeBuffer(FD3D12Device* Parent, uint32 Size, uint32 Stride, FRICreationDescriptor resourceData) :
	FD3D12Buffer(Parent),
	FRIComputeBuffer(Size, Stride, EFRIUsage::Default, EFRIAccess::None)
{
	CreateBuffer(resourceData, Size * Stride, 4, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

FD3D12ConstantBuffer::FD3D12ConstantBuffer(FD3D12Device* Parent, uint32 Size, FRICreationDescriptor resourceData) :
	FD3D12Buffer(Parent),
	FRIConstantBuffer(Size, 1, EFRIUsage::Default, EFRIAccess::None)
{
	resourceData.ByteSize = Size;
	CreateBuffer(resourceData, Size, 256);

	View = DX12::Cast<FD3D12ConstantBufferView>(Parent->Context->GetFRIDynamic()->CreateConstantBufferView(this));
}