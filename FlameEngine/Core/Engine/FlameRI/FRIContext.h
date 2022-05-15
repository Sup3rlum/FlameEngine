#pragma once


#include "Core/Common/CoreCommon.h"

#include "Core/Engine/GameSystem/KeyInputBinding.h"
#include "Viewport.h"
#include "FRIDynamicAllocator.h"

enum class ERenderingContextPixelFormat
{
	
};






struct FRIRenderingContextDescription
{
	uint32 Width;
	uint32 Height;
	uint32 SampleCount;

	EFRIRendererFramework RenderFramework;
	ERenderingContextPixelFormat PixelFormat;
	bool IsFullscreen;
	class Win32Context* WinContext;

};


class FRIContext
{
protected:

	bool isActive;
	FRIDynamicAllocator* dynamicAllocator;


	FRIContext(FRIRenderingContextDescription description, FRIContext* contextToCopy = NULL) :
		InstanceDescription(description),
		dynamicAllocator(NULL),
		isActive(false)
	{

	}


public:
	FRIRenderingContextDescription InstanceDescription;

	FKeyEventBindingDelegate InputHandlerDelegate;
	FMouseKeyEventBindingDelegate InputHandlerDelegate2;
	


	void PollCloseEvent()
	{
		isActive = false;
	}

	bool IsActive() const
	{
		return isActive;
	}


	FORCEINLINE FRIDynamicAllocator* GetFRIDynamic() const
	{
		return dynamicAllocator;
	}

	virtual void Initialize() = 0;
	virtual void* GetPlatformSpecificHandle() = 0;
	virtual void HandleEvents() = 0;
	virtual void SwapBuffers() = 0;



	virtual FVector2 GetCursorPosition() = 0;
	virtual void SetCursorPosition(FVector2 pos) = 0;
	virtual void ShowCursor() = 0;
	virtual void HideCursor() = 0;

	virtual FViewportRect GetViewport() = 0;
};