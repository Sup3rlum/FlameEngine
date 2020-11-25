#pragma once

#include "../../dll/nchfx.h"

#include "../../Graphics/Texture.h"

EXPORT_CLASS Material
{
public:

	Material();


	Texture* mColorMap;
	Texture* mNormalMap;
	Texture* mHeightMap;


	float mSpecularIndex;
	float mDiffuseIndex;
	float mFresnelIndex;

};

