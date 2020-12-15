#pragma once

#include "../../../dll/nchfx.h"

#include "../../Common/Texture.h"

EXPORT(class, MaterialBase)
{
public:

	MaterialBase();


	float mSpecularIndex;
	float mDiffuseIndex;
	float mFresnelIndex;

	void SetMap(STRING name, Texture* map);
	Texture* GetMap(STRING name);


private:
	std::map<STRING, Texture*> _internal;

};