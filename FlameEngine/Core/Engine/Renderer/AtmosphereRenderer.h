#pragma once


#include "RenderModule.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"
#include "Common/RenderUtil.h"
#include "Core/Framework/IO/FileStream.h"

#include "../GameSystem/Mesh.h"
#include "../ContentSystem/Client/AssetImportScripts/Mesh.h"
#include "../ContentSystem/Client/AssetImportScripts/ShaderLibrary.h"


struct AtmosphereDescription
{
	FVector3 RayleighScatteringCoefficient;
	float MieScatteringCoeffcient;
	float SunIntensity;

	AtmosphereDescription(FVector3 rsc, float msc, float si) :
		RayleighScatteringCoefficient(rsc),
		MieScatteringCoeffcient(msc),
		SunIntensity(si)
	{
	}

	AtmosphereDescription() : AtmosphereDescription(0, 0, 0)
	{

	}
};


struct AtmosphereScale
{
	float RayleighScale;
	float MieScale;
	float GFactor;

	AtmosphereScale(float rs, float ms, float g) :
		RayleighScale(rs),
		MieScale(ms),
		GFactor(g)
	{

	}
	AtmosphereScale() : AtmosphereScale(0, 0, 0)
	{

	}
};

struct AtmospherePresets
{
	//inline static AtmosphereDescription Mercury;
	inline static AtmosphereDescription Venus = AtmosphereDescription(FVector3(2.67e-5, 6.23e-4, 1.52e-4), 21e-6, 30);
	inline static AtmosphereDescription Earth = AtmosphereDescription(FVector3(5.5e-6, 13.0e-6, 22.4e-6), 21e-6, 22);
	inline static AtmosphereDescription Mars = AtmosphereDescription(FVector3(19.918e-6, 13.57e-6, 5.75e-6), 21e-6, 5);
	//inline static AtmosphereDescription Jupiter;
	inline static AtmosphereDescription Saturn = AtmosphereDescription(FVector3(3.45e-7, 8.07e-7, 1.97e-6), 21e-6, 0.22);
};

class AtmosphereRenderer : RenderModule
{
public:

	void CreateResources(FRIContext* renderContext);
	void RecreateResources(FRIContext* renderContext, FRIContext* prevContext = 0);
	void RenderSkySphere(FRICommandList& cmdList, CameraComponent& cameraRef, DirectionalLight& cameraTransform);
	void SetRenderStates(FRICommandList& cmdList);

	FRIShaderPipeline* pipeline;
	FRIUniformBuffer* ConstantBuffer;
	
	FRIRasterizerState* AtmosphereRasterizer;
	FRIDepthStencilState* AtmosphereDepthState;
	Mesh renderSphere;

	bool Enabled = true;


	AtmosphereDescription Atmosphere;
	AtmosphereScale Scale;
};