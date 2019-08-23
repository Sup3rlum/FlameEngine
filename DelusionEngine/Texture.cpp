#include "Texture.h"


Texture::Texture(const char* _path)
{
	glGenTextures(1, &_handle);
	glBindTexture(GL_TEXTURE_2D, _handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	_dataInternal = stbi_load(_path, &_width, &_height, &_channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _dataInternal);
	glGenerateMipmap(GL_TEXTURE_2D);

}

Texture::Texture(GLuint width, GLuint height, GLint colorFormat, GLint outerColorFormat, GLint dataFormat, bool mimmaps)
{
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

}
void Texture::SetParameter(GLenum _n, GLenum _v)
{
	glTexParameteri(GL_TEXTURE_2D, _n, _v);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, _handle);
}
void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}


vec4 Texture::GetData(int x, int y)
{
	unsigned char* _offs = (unsigned char*)_dataInternal + (x + _height * y) * 4;

	return vec4
	(
		_offs[0],
		_offs[1],
		_offs[2],
		_offs[3]
	);
}