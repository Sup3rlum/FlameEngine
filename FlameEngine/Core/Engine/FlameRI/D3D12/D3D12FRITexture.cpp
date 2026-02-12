#include "D3D12FRICommon.h"

void FD3D12Texture::CreateTexture(FD3D12Device* Parent, uint32 Width, uint32 Height, uint32 Depth, uint32 MipLevels, DXGI_FORMAT Format, D3D12_RESOURCE_DIMENSION Dimension, FRICreationDescriptor dataDesc[])
{
    this->Format = Format;
    auto Device = Parent->GetDevice();

    const auto DefaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    const auto UploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    D3D12_RESOURCE_DESC TextureDesc = {};
    TextureDesc.Dimension = Dimension;
    TextureDesc.Alignment = 0;
    TextureDesc.Width = Width;
    TextureDesc.Height = Height;
    TextureDesc.DepthOrArraySize = Depth;
    TextureDesc.MipLevels = MipLevels;
    TextureDesc.Format = Format;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    TextureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    
    if (Format == DXGI_FORMAT_D32_FLOAT)
    {
        TextureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }
    else
    {
        TextureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    }

    D3D12_CLEAR_VALUE OptimizedClearValue;
    ZeroMem(OptimizedClearValue);
    OptimizedClearValue.Format = Format;

    if (Format == DXGI_FORMAT_D32_FLOAT)
    {
        OptimizedClearValue.DepthStencil.Depth = 1.0f;
        OptimizedClearValue.DepthStencil.Stencil = 0;
    }

    TargetResource = new FD3D12Resource(
        "Unnamed Texture",
        Parent,
        DefaultHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        TextureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        &OptimizedClearValue
    );

    UINT64 textureUploadBufferSize;
    // this function gets the size an upload buffer needs to be to upload a texture to the gpu.
    // each row must be 256 byte aligned except for the last row, which can just be the size in bytes of the row
    // eg. textureUploadBufferSize = ((((width * numBytesPerPixel) + 255) & ~255) * (height - 1)) + (width * numBytesPerPixel);
    //textureUploadBufferSize = (((imageBytesPerRow + 255) & ~255) * (textureDesc.Height - 1)) + imageBytesPerRow;
    Device->GetCopyableFootprints(&TextureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);
    const auto TextureBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize);

    IntermediateResource = new FD3D12Resource(
        "Unnamed Texture (Upload)",
        Parent,
        UploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        TextureBufferDesc, 
        D3D12_RESOURCE_STATE_GENERIC_READ,
        &OptimizedClearValue
    );

    if (dataDesc)
    {
        D3D12_SUBRESOURCE_DATA* textureData = {};


        uint32 NumLayers = Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D ? 1 : Depth;
        uint32 ArrayDepth = Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D ? Depth : 1;

        D3DConvertDescriptorToSubresource(Height, ArrayDepth, NumLayers, MipLevels, dataDesc, &textureData);
        UpdateSubresources(MipLevels, textureData);
    }

    if (Format == DXGI_FORMAT_D32_FLOAT)
    {
        TargetResource->TransitionState(D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    }
    else
    {
        TargetResource->TransitionState(D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
}


FD3D12Texture2D::FD3D12Texture2D(FD3D12Device* Parent, uint32 Width, uint32 Height, uint32 MipLevels, DXGI_FORMAT format, FRICreationDescriptor dataDesc[]) :
    FD3D12Texture(Parent),
    FRITexture2D(Width, Height, MipLevels)
{
    CreateTexture(Parent, Width, Height, 1, MipLevels, format, D3D12_RESOURCE_DIMENSION_TEXTURE2D, dataDesc);
}


FD3D12Texture2DArray::FD3D12Texture2DArray(FD3D12Device* Parent, uint32 Width, uint32 Height, uint32 ArraySize, uint32 MipLevels, DXGI_FORMAT format, FRICreationDescriptor dataDesc[]) :
    FD3D12Texture(Parent),
    FRITexture2DArray(Width, Height, ArraySize, MipLevels)
{
    CreateTexture(Parent, Width, Height, ArraySize, MipLevels, format, D3D12_RESOURCE_DIMENSION_TEXTURE2D, dataDesc);
}

FD3D12TextureCubeMap::FD3D12TextureCubeMap(FD3D12Device* Parent, uint32 Width, uint32 MipLevels, DXGI_FORMAT format, FRICreationDescriptor dataDesc[]) :
    FD3D12Texture(Parent),
    FRITextureCubeMap(Width, Width, MipLevels)
{
    CreateTexture(Parent, Width, Width, 6, MipLevels, format, D3D12_RESOURCE_DIMENSION_TEXTURE2D, dataDesc);
}


FD3D12Texture3D::FD3D12Texture3D(FD3D12Device* Parent, uint32 Width, uint32 Height, uint32 Depth, uint32 MipLevels, DXGI_FORMAT format, FRICreationDescriptor dataDesc[]) :
    FD3D12Texture(Parent),
    FRITexture3D(Width, Height, Depth, MipLevels)
{
    CreateTexture(Parent, Width, Width, Depth, MipLevels, format, D3D12_RESOURCE_DIMENSION_TEXTURE3D, dataDesc);
}
