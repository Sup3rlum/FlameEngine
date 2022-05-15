#pragma once


#include "RenderModule.h"
#include "Common/Geometry/VertexComponent.h"
#include "../GameSystem/Common/Scene.h"
#include "Common/RenderUtil.h"
#include "Core/Framework/IO/FileStream.h"

#include "../GameSystem/Mesh.h"
#include "../ContentSystem/ImportScripts/Mesh.h"
#include "../ContentSystem/ImportScripts/ShaderLibrary.h"
#include "../PCI/PCI.h"


class AtmosphereRenderer : public RenderModule, public IProperties
{
public:

	PropertyVector3(RayleighScatteringCoefficient, FVector3(5.5e-6, 13.0e-6, 22.4e-6));
	PropertyFloat(MieScatteringCoeffcient, 21e-6);
	PropertyFloat(SunIntensity, 22);
	PropertyFloat(RayleighScale, 8e3);
	PropertyFloat(MieScale, 1.2e3);
	PropertyFloat(GFactor, 0.758f);
	PropertyFloat(PlanetRadius, 6371e3);
	PropertyFloat(AtmosphereThickness, 100e3);
	PropertyInt(PrimarySteps, 32);
	PropertyInt(SecondarySteps, 8);

	PropertyBool(Enabled, true);


	void CreateResources(FRIContext* renderContext);
	void RecreateResources(FRIContext* renderContext, FRIContext* prevContext = 0);
	void RenderSkySphere(FRICommandList& cmdList, const Camera& cameraRef, const DirectionalLight& cameraTransform);
	void SetRenderStates(FRICommandList& cmdList);

private:
	FRIShaderPipelineRef pipeline;
	FRIStageBuffer SettingsBuffer;
	
	FRIRasterizerStateRef AtmRasterizer;
	FRIDepthStencilStateRef AtmDepthState;
};