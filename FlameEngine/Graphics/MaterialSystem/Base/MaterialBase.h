#pragma once

#include "../../../dll/nchfx.h"

#include "../../Common/Texture.h"

EXPORT(class, MaterialBase)
{
public:

	MaterialBase();


	Texture* mColorMap;
	Texture* mNormalMap;
	Texture* mHeightMap;


	float mSpecularIndex;
	float mDiffuseIndex;
	float mFresnelIndex;

};