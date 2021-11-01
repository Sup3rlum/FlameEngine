#include "UXFRISurface.h"


UXFRISurfaceFactory::UXFRISurfaceFactory(FRIContext* context) :
	FriContext(context)
{

}

UXFRISurfaceFactory::~UXFRISurfaceFactory()
{

}

ultralight::Surface* UXFRISurfaceFactory::CreateSurface(uint32 width, uint32 height)
{
	return new UltraLightFRISurface(FriContext, width, height);
}



void UXFRISurfaceFactory::DestroySurface(ultralight::Surface* surface)
{
	delete static_cast<UltraLightFRISurface*>(surface);
}