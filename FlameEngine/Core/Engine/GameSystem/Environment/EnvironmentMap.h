#pragma once

#include "Core/Engine/FlameRI/FRI.h"
#include "../EntityComponent/EntityComponent.h"

struct EnvironmentMap
{
	FRITextureCubeMap* Skymap;
	FRITextureCubeMap* Specular;
	FRITextureCubeMap* Irradiance;


	EnvironmentMap(FRITextureCubeMap* skymap, FRITextureCubeMap* specular, FRITextureCubeMap* irradiance) : 
		Skymap(skymap),
		Specular(specular),
		Irradiance(irradiance)
	{
	}

	void DrawSkySphere(FRICommandList& cmdList, FRIShaderPipeline* skyShader);

};