#pragma once


#include "Core/Common/CoreCommon.h"

#include "Core/Engine/GameSystem/KeyInputBinding.h"

enum class ERenderingContextPixelFormat
{
	
};

struct FRIRenderingContextDescription
{
	uint32 Width;
	uint32 Height;
	uint32 SampleCount;

	ERenderingContextPixelFormat PixelFormat;
	bool IsFullscreen;

};


class FRIContext
{
public:
	FRIRenderingContextDescription InstanceDescription;
	FKeyEventBindingDelegate InputHandlerDelegate;

	FRIContext(FRIRenderingContextDescription description, FRIContext* contextToCopy = NULL ) :
		InstanceDescription(description)
	{
		
	}


	virtual void Initialize() = 0;

	virtual void* GetPlatformSpecificHandle() = 0;

	virtual bool HandleEvents() = 0;

	virtual void SwapBuffers() = 0;
};