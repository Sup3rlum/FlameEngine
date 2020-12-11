#pragma once

#include "../../dll/nchfx.h"
#include "../../util/stb_image.h"
#include "../../Mathematics/Module.h"
#include "TextureFiltering.h"

#include "ITextureBase.h"


EXPORT(class,  MultisampleTexture) : public ITextureBase
{
public:

	MultisampleTexture(STRING filepath, int sampleCount);
	MultisampleTexture(GLuint with, GLuint height, GLint _colorFormat, int sampleCount);


private:
	int _width;
	int _height;
	int _channels;

	int samples;

};

