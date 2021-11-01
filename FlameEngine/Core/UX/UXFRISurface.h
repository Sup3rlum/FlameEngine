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
    virtual FRITexture2D* GetTexture() = 0;

    uint32_t Width;
    uint32_t Height;
    uint32_t RowBytes;
    uint32_t ByteSize;
    FRIContext* renderContext;


    FRITexture2D* TextureBuffer;
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

        FRICommandList cmdList(renderContext->GetFRIDynamic());

        if (TextureBuffer && Width == width && Height == height)
            return;

        if (TextureBuffer)
        {
            delete TextureBuffer;
            TextureBuffer = NULL;
        }

        Width = width;
        Height = height;
        RowBytes = Width * 4;
        ByteSize = RowBytes * Height;
        
        TextureBuffer = cmdList.GetDynamic()->CreateTexture2D(Width, Height, 1, EFRITextureFormat::RGBA8UNORM, FRIColorDataFormat(EFRIChannels::RGBA, EFRIPixelStorage::UnsignedByte), FRICreationDescriptor(NULL, ByteSize), true);

        UlBitmap = ultralight::Bitmap::Create(Width, Height, ultralight::kBitmapFormat_BGRA8_UNORM_SRGB);
    }

    FRITexture2D* GetTexture()
    {
        if (!dirty_bounds().IsEmpty()) 
        {
            FRICommandList cmdList(renderContext->GetFRIDynamic());

            void* pixels = UlBitmap->LockPixels();         
            cmdList.GetDynamic()->Texture2DSubdata(TextureBuffer, FRIUpdateDescriptor(pixels, 0, ByteSize));
            UlBitmap->UnlockPixels();

            ClearDirtyBounds();
        }

        return TextureBuffer;
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