#include "Texture.h"


Texture::Texture(STRING _path)
{

	TextureType = GL_TEXTURE_2D;

	glGenTextures(1, &_handle);
	glBindTexture(GL_TEXTURE_2D, _handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	_dataInternal = stbi_load(_path.c_str(), &_width, &_height, &_channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _dataInternal);
	glGenerateMipmap(GL_TEXTURE_2D);


	IsMultiSampled = false;
}

Texture::Texture(GLuint width, GLuint height, GLint colorFormat, GLint outerColorFormat, GLint dataFormat, bool mimmaps)
{

	TextureType = GL_TEXTURE_2D;

	_width = width;
	_height = height;

	_colorFormat = colorFormat;
	_outerColorFormat = outerColorFormat;
	_dataFormat = dataFormat;

	glGenTextures(1, &_handle);
	glBindTexture(GL_TEXTURE_2D, _handle);

	glTexImage2D(GL_TEXTURE_2D, 0, _colorFormat, width, height, 0, _outerColorFormat, _dataFormat, NULL);

	if (mimmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	IsMultiSampled = false;
}

Texture::Texture(GLuint width, GLuint height, GLint colorFormat, GLint outerColorFormat, GLint dataFormat, bool mimmaps, int sampleCount)
{

	TextureType = GL_TEXTURE_2D_MULTISAMPLE;

	_width = width;
	_height = height;

	_colorFormat = colorFormat;
	_outerColorFormat = outerColorFormat;
	_dataFormat = dataFormat;

	glGenTextures(1, &_handle);
	glBindTexture(TextureType, _handle);

	glTexImage2DMultisample(TextureType, sampleCount, _colorFormat, width, height, GL_FALSE);

	if (mimmaps)
	{
		glGenerateMipmap(TextureType);
	}


	IsMultiSampled = true;
}

void Texture::SetParameter(GLenum _n, GLenum _v)
{
	glTexParameteri(TextureType, _n, _v);
}
void Texture::SetParameterf(GLenum _n, float _v)
{
	glTexParameterf(TextureType, _n, _v);
}

void Texture::Bind()
{
	glBindTexture(TextureType, _handle);
}
void Texture::Unbind()
{
	glBindTexture(TextureType, 0);
}


fVector4 Texture::GetData(int x, int y)
{
	unsigned char* _offs = (unsigned char*)_dataInternal + (x + _height * y) * 4;

	return fVector4
	(
		_offs[0],
		_offs[1],
		_offs[2],
		_offs[3]
	);
}