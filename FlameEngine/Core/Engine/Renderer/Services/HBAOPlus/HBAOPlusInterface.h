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
struct FResourceTexture2D;
struct FResourceFrameBuffer;


class HBAOPlus
{
public:

	HBAOPlus(FHBAOParameters params) : 
		params(params)
	{}

	virtual void RenderAO(FResourceTexture2D* depthTex, FResourceTexture2D* normTex, FResourceFrameBuffer* outputFBO, const FMatrix4& projection, const FMatrix4& view) = 0;


	static HBAOPlus* Allocate(FRIContext* renderContext, FHBAOParameters params);

	FHBAOParameters params;
};
