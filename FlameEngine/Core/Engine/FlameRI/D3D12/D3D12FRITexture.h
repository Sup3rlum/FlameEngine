#pragma once


class FD3D12Texture : public FD3D12TransientResource
{
protected:
	FD3D12Texture(FD3D12Device* Parent) : FD3D12TransientResource(Parent, D3D12_RESOURCE_STATE_GENERIC_READ), Format(DXGI_FORMAT_UNKNOWN) {}
	void CreateTexture(FD3D12Device* Parent, uint32 Width, uint32 Height, uint32 Depth, uint32 MipLevels, DXGI_FORMAT format, D3D12_RESOURCE_DIMENSION Dimension, FRICreationDescriptor dataDesc[]);
	DXGI_FORMAT Format;

	friend class D3D12FRIDynamicAllocator;

};

class FD3D12Texture2D : public FRITexture2D, public FD3D12Texture
{
public:
	FD3D12Texture2D(FD3D12Device* Parent, uint32 Width, uint32 Height, uint32 MipLevel, DXGI_FORMAT format, FRICreationDescriptor dataDesc[]);
	void Rename(const FString& Name) override { RenameResources(Name); }
};

class FD3D12Texture2DArray : public FRITexture2DArray, public FD3D12Texture
{
public:
	FD3D12Texture2DArray(FD3D12Device* Parent, uint32 Width, uint32 Height, uint32 ArraySize, uint32 MipLevel, DXGI_FORMAT format, FRICreationDescriptor dataDesc[]);
	void Rename(const FString& Name) override { RenameResources(Name); }
};

class FD3D12TextureCubeMap : public FRITextureCubeMap, public FD3D12Texture
{
public:
	FD3D12TextureCubeMap(FD3D12Device* Parent, uint32 Width, uint32 MipLevel, DXGI_FORMAT format, FRICreationDescriptor dataDesc[]);
	void Rename(const FString& Name) override { RenameResources(Name); }
};

class FD3D12Texture3D : public FRITexture3D, public FD3D12Texture
{
public:
	FD3D12Texture3D(FD3D12Device* Parent, uint32 Width, uint32 Height, uint32 MipLevel, uint32 Depth, DXGI_FORMAT format, FRICreationDescriptor dataDesc[]);
	void Rename(const FString& Name) override { RenameResources(Name); }
};