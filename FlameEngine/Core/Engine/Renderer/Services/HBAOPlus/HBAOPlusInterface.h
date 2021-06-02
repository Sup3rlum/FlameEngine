#pragma once



#include "Core/Math/Module.h"


struct FHBAOParameters
{
	float Radius;
	float Bias;
	float NearAO;
	float FarAO;

	bool BackgroundAOEnable;
	float BackgroundAO_ViewDepth;

	bool ForegroundAOEnable;
	float ForegroundAO_ViewDepth;

	float PowerExponent;
	bool BlurEnable;
	float BlurSharpness;
};

struct FRIContext;
struct FRITexture2D;
struct FRIFrameBuffer;


class HBAOPlus
{
public:

	HBAOPlus(FHBAOParameters params) : 
		params(params)
	{}

	virtual void RenderAO(FRITexture2D* depthTex, FRITexture2D* normTex, FRIFrameBuffer* outputFBO, const FMatrix4& projection, const FMatrix4& view) = 0;


	static HBAOPlus* Allocate(FRIContext* renderContext, FHBAOParameters params);

	FHBAOParameters params;
};
