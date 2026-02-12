#pragma once


class FD3D12Buffer : public FD3D12TransientResource
{
public:

protected:
	FD3D12Buffer(FD3D12Device* Parent) : FD3D12TransientResource(Parent, D3D12_RESOURCE_STATE_GENERIC_READ) {}
	void CreateBuffer(FRICreationDescriptor dataDesc, uint32 ByteSize, uint64 Alignment, bool bAllowUAV=true, D3D12_RESOURCE_STATES workingState = D3D12_RESOURCE_STATE_GENERIC_READ);

};


class FD3D12VertexBuffer : public FD3D12Buffer, public FRIVertexBuffer
{
public:
	D3D12_VERTEX_BUFFER_VIEW View;
	FD3D12VertexBuffer(FD3D12Device* Parent, uint32 Size, uint32 Stride, FRICreationDescriptor resourceData);

	auto& GetView() const { return View; }
	void Rename(const FString& Name) override { RenameResources(Name); }

};

class FD3D12IndexBuffer : public FD3D12Buffer, public FRIIndexBuffer
{
public:
	D3D12_INDEX_BUFFER_VIEW View;
	FD3D12IndexBuffer(FD3D12Device* Parent, uint32 Size, uint32 Stride, FRICreationDescriptor resourceData);

	auto& GetView() const { return View; }
	void Rename(const FString& Name) override { RenameResources(Name); }
};

class FD3D12ComputeBuffer : public FD3D12Buffer, public FRIComputeBuffer
{
public:
	FD3D12ComputeBuffer(FD3D12Device* Parent, uint32 Size, uint32 Stride, FRICreationDescriptor resourceData);
	void Rename(const FString& Name) override { RenameResources(Name); }

};

class FD3D12ConstantBuffer : public FD3D12Buffer, public FRIConstantBuffer
{

	FD3D12ConstantBufferView* View;

public:
	FD3D12ConstantBuffer(FD3D12Device* Parent, uint32 Size, FRICreationDescriptor resourceData);

	FRIConstantBufferView* GetView() override {	return View; }
	void Rename(const FString& Name) override { RenameResources(Name); }

};