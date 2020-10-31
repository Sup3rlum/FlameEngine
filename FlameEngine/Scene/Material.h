#pragma once

#include "../dll/nchfx.h"

#include "../Graphics/Texture.h"

EXPORT_CLASS Material
{
public:

	Material();

	Texture* colorMap;

	float roughness;
	float specular;
};

