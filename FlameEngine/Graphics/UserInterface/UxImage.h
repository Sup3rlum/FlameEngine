#pragma once

#include "../../dll/nchfx.h"

#include "UxComponent.h"

EXPORT(class, UxImage) : UxComponent
{
public:

	UxImage(fVector2 position, fVector2 size, Texture* texture);

	fVector2 Position;
	fVector2 Size;

	void Render() override;
	void Update() override;

	float pxBorderRadius;
private:
	Texture* mTexture;
	Shader* mImageShader;
};

