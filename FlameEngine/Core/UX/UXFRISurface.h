#pragma once


#include "Core/Engine/FlameRI/FRI.h"
#include <Ultralight/platform/Surface.h>



class UXFRISurface
{
protected:
    UXFRISurface(FRIContext* context, uint32 width, uint32 height) :
        renderContext(context),
        Width(width),
        Height(height),
        RowBytes(Width * 4),
        ByteSize(Width * Height * 4),
        TextureBuffer(NULL)
    {

    }
public:
    virtual FRIShaderResourceView* GetView() = 0;

    uint32_t Width;
    uint32_t Height;
    uint32_t RowBytes;
    uint32_t ByteSize;
    FRIContext* renderContext;


    FRITexture2D* TextureBuffer;
    FRIShaderResourceView* SurfaceSRV;
};

class UltraLightFRISurface : public ultralight::Surface , public UXFRISurface
{
public:
    UltraLightFRISurface(FRIContext* context, uint32 width, uint32 height) :
		UXFRISurface(context, width, height)
	{
		Resize(width, height);
	}

    ~UltraLightFRISurface()
    {
        delete TextureBuffer;
    }

	virtual uint32_t width() const override { return Width; }
	virtual uint32_t height() const override { return Height; }
	virtual uint32_t row_bytes() const override { return RowBytes; }
	virtual size_t size() const override { return ByteSize; }

    virtual void* LockPixels() override 
    {
        return UlBitmap->LockPixels();
    }

    virtual void UnlockPixels() override
    {
        UlBitmap->UnlockPixels();
    }

    virtual void Resize(uint32_t width, uint32_t height) override 
    {
        FRICommandList cmdList(renderContext->GetCommandContext(0));
        auto Allocator = renderContext->GetFRIDynamic();
        cmdList.Open();

        if (TextureBuffer && Width == width && Height == height)
            return;

        if (TextureBuffer)
        {
            delete TextureBuffer;
            TextureBuffer = NULL;
        }
        if (SurfaceSRV)
        {
            delete SurfaceSRV;
            SurfaceSRV = NULL;
        }

        Width = width;
        Height = height;
        RowBytes = Width * 4;
        ByteSize = RowBytes * Height;
        
        TextureBuffer = Allocator->CreateTexture2D(Width, Height, 1, EFRIAccess::Write, EFRITextureFormat::RGBA8UNORM);
        SurfaceSRV = Allocator->CreateShaderResourceView(TextureBuffer);
        UlBitmap = ultralight::Bitmap::Create(Width, Height, ultralight::kBitmapFormat_BGRA8_UNORM_SRGB);

        cmdList.CloseAndExecute();
    }

    FRIShaderResourceView* GetView()
    {
        if (!dirty_bounds().IsEmpty()) 
        {

            void* pixels = UlBitmap->LockPixels();         
            FRICommandList cmdList(renderContext->GetCommandContext(0));
            cmdList.ResourceSubdata(TextureBuffer, FRIUpdateDescriptor(pixels, 0, ByteSize, RowBytes));
            UlBitmap->UnlockPixels();

            ClearDirtyBounds();
        }

        return SurfaceSRV;
    }


    ultralight::RefPtr<ultralight::Bitmap> UlBitmap;

};

class UXFRISurfaceFactory : public ultralight::SurfaceFactory
{
public:
    UXFRISurfaceFactory(FRIContext* context);
    virtual ~UXFRISurfaceFactory();

    ultralight::Surface* CreateSurface(uint32 width, uint32 height) override;

    void DestroySurface(ultralight::Surface* surface) override;

    FRIContext* FriContext;

};