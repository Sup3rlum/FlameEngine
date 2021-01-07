#include "TextureArray.h"


TextureArray::TextureArray(GLuint _width, GLuint _height, GLuint _depth, GLint _colorFormat, GLint _outerColorFormat, GLint _dataFormat)
{
	TextureType = GL_TEXTURE_2D_ARRAY;

	width = _width;
	height = _height;
	depth = _depth;

	colorFormat = _colorFormat;
	outerColorFormat = _outerColorFormat;

	glGenTextures(1, &handle);
	glBindTexture(TextureType, handle);

	glTexImage3D(TextureType, 0, _colorFormat, width, height, depth, 0, _outerColorFormat, _dataFormat, NULL);
}

void TextureArray::SetCompareFunction(TextureCompareFunction func)
{
	Bind();
	SetParameter(GL_TEXTURE_COMPARE_FUNC, (int)func);
}
