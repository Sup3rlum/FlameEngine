#pragma once

#include "../../../dll/nchfx.h"
#include "../../../Graphics/Common/Texture.h"
#include "../../../Graphics/Common/Query.h"
#include "../Camera.h"

EXPORT(struct, LensFlareComponent)
{
public:
	Texture* flareTexture;
	float scale;

	LensFlareComponent(Texture * t, float s) : flareTexture(t), scale(s) {}

};


EXPORT(class, LensFlare)
{
public:
	LensFlare(Context* attachedContext);

	void Render(Camera * cam);


	float intensity;

	fVector3 sunDirection;
	fVector2 screenPosition;

	std::vector<LensFlareComponent> flares;

};

