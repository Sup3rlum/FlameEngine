#pragma once
/*

#include "RenderingServiceBase.h"

EXPORT_ENUM GaussianBlurStrength
{
	KERNEL_5 = 0,
	KERNEL_11 = 1,
	KERNEL_17 = 2,
	KERNEL_29 = 3,
	KERNEL_63 = 4
};

EXPORT(class,  GaussianBlurService) : public RenderingServiceBase
{
public:
	GaussianBlurService(Context* context, bool halfRes);

	void ApplyFilter(FrameBuffer* frameBuffer, Texture* texture);

	Program* mShader;
	FrameBuffer* mFrameBuffer;
	Texture* mBuffer;

	GaussianBlurStrength mBlurStrength = GaussianBlurStrength::KERNEL_5;
};

*/