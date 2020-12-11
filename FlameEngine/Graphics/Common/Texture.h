#pragma once

#include "../../dll/nchfx.h"
#include "../../util/stb_image.h"
#include "../../Mathematics/Module.h"
#include "TextureFiltering.h"


#include "ITextureBase.h"

EXPORT(class,  Texture) : public ITextureBase
{
public:

	Texture(STRING filepath);
	Texture(GLuint with, GLuint height, GLint _colorFormat, GLint outerColorFormat, GLint dataFormat);



	void SetData(void* _data) override;


private:
	int width;
	int height;

};

