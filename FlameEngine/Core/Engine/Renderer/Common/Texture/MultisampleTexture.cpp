#include "MultisampleTexture.h"


MultisampleTexture::MultisampleTexture(STRING _path, int sampleCount)
{
	samples = sampleCount;

	TextureType = GL_TEXTURE_2D_MULTISAMPLE;

	glGenTextures(1, &handle);
	glBindTexture(TextureType, handle);

	SetWrappingMode(TextureWrapping::REPEAT);
	SetFilteringMode(TextureFiltering::NONE);


	//glTexImage2DMultisample(TextureType, samples, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _dataInternal);
	glGenerateMipmap(TextureType);
}

MultisampleTexture::MultisampleTexture(GLuint width, GLuint height, GLint colorFormat, int sampleCount)
{
	samples = sampleCount;

	TextureType = GL_TEXTURE_2D_MULTISAMPLE;

	_width = width;
	_height = height;

	this->colorFormat = colorFormat;


	glGenTextures(1, &handle);
	glBindTexture(TextureType, handle);

	glTexImage2DMultisample(TextureType, samples, colorFormat, width, height, false);
	glGenerateMipmap(TextureType);
	
}
