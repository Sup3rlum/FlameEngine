#pragma once

#include "Core/Engine/FlameRI/FRI.h"
#include "../EntityComponent/EntityComponent.h"

struct EnvironmentMap
{
	FRITextureCubeMap* Specular;
	FRITextureCubeMap* Irradiance;

	EnvironmentMap(FRITextureCubeMap* specular, FRITextureCubeMap* irradiance) : 
		Specular(specular),
		Irradiance(irradiance)
	{
	}
};