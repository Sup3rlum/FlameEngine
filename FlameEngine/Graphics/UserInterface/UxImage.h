#pragma once

#include "../../dll/nchfx.h"

#include "UxComponent.h"

EXPORT(class, UxImage) : UxComponent
{
public:

	UxImage(FVector2 position, FVector2 size, Texture* texture);

	FVector2 Position;
	FVector2 Size;

	void Render() override;
	void Update() override;

	float pxBorderRadius;
private:
	Texture* mTexture;
	Program* mImageShader;
};

