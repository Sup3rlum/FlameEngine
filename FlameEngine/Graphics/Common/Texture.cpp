#include "Texture.h"


Texture::Texture(STRING _path)
{

	TextureType = GL_TEXTURE_2D;

	glGenTextures(1, &handle);
	glBindTexture(TextureType, handle);

	SetWrappingMode(TextureWrapping::REPEAT);
	SetFilteringMode(TextureFiltering::ANISOTROPIC_16);


	void* _dataInternal = stbi_load(_path.c_str(), &width, &height, &channels, 0);


	GLuint colorFormat = channels == 4 ? GL_RGBA : GL_RGB;

	glTexImage2D(TextureType, 0, colorFormat, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, _dataInternal);
	glGenerateMipmap(TextureType);

}

Texture::Texture(GLuint width, GLuint height, GLint colorFormat, GLint outerColorFormat, GLint dataFormat)
{

	TextureType = GL_TEXTURE_2D;

	this->width = width;
	this->height = height;

	this->colorFormat = colorFormat;
	this->outerColorFormat = outerColorFormat;
	this->dataFormat = dataFormat;

	glGenTextures(1, &handle);
	glBindTexture(TextureType, handle);

	glTexImage2D(TextureType, 0, colorFormat, width, height, 0, outerColorFormat, dataFormat, NULL);


}


void Texture::SetData(void* data)
{
	Bind();
	glTexImage2D(TextureType, 0, colorFormat, width, height, 0, outerColorFormat, dataFormat, data);
}