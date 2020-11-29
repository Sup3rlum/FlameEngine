#pragma once



#include "UxComponent.h"

#include "../../Mathematics/Module.h"


EXPORT(class,  UxFrame) : public UxComponent
{
public:

	UxFrame(fVector2 position, fVector2 size);

	fVector2 Position;
	fVector2 Size;

	void Render() override;
	void Update() override;

	Color color;
	float pxBorderRadius;
private:
	Texture* mTexture;
	Shader* mFrameShader;


};

