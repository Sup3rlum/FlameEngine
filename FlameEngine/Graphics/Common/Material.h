#pragma once

#include "../../dll/nchfx.h"

#include "Texture.h"

EXPORT(class, Material)
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