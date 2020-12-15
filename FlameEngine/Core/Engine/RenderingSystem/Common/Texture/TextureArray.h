#pragma once

#include "../../dll/nchfx.h"
#include "../../util/stb_image.h"
#include "../../Mathematics/Module.h"
#include "TextureFiltering.h"

#include "ITextureBase.h"

EXPORT(enum class, TextureCompareFunction)
{
	Always = 0x0207,
	Never = 0x0200,
	Less = 0x0201,
	Equal = 0x0202,
	LessOrEqual = 0x0203,
	Greater = 0x0204,
	GeaterOrEqal = 0x0206,
	NotEqual = 0x0205
		
};


EXPORT(class, TextureArray) : public ITextureBase
{
public:

	TextureArray(GLuint with, GLuint height, GLuint depth, GLint _colorFormat, GLint outerColorFormat, GLint dataformat);

	void SetCompareFunction(TextureCompareFunction func);

private:
	
	uint width;
	uint height;
	uint depth;
};

