#include "Texture.h"


Texture::Texture(STRING _path)
{

	TextureType = GL_TEXTURE_2D;

	glGenTextures(1, &_handle);
	glBindTexture(TextureType, _handle);

	SetWrappingMode(TextureWrapping::REPEAT);
	SetFilteringMode(TextureFiltering::NONE);


	_dataInternal = stbi_load(_path.c_str(), &_width, &_height, &_channels, 0);

	glTexImage2D(TextureType, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _dataInternal);
	glGenerateMipmap(TextureType);

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
	glBindTexture(TextureType, _handle);

	glTexImage2D(TextureType, 0, _colorFormat, width, height, 0, _outerColorFormat, _dataFormat, NULL);

	if (mimmaps)
	{
		glGenerateMipmap(TextureType);
	}

}


// ------ Filtering


void Texture::SetFilteringMode(TextureFiltering type)
{
	mFilteringType = type;

	Bind();



	switch (type)
	{
	default:
	case TextureFiltering::NONE:
		SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case TextureFiltering::BILINEAR:
		SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case TextureFiltering::TRILINEAR:
		SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	case TextureFiltering::ANISOTROPIC_8:
		SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		SetParameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
		break;
	case TextureFiltering::ANISOTROPIC_16:
		SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		SetParameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		break;
	}
}

void Texture::SetWrappingMode(TextureWrapping type)
{
	mWrappingMode = type;

	Bind();

	SetParameter(GL_TEXTURE_WRAP_S, type);
	SetParameter(GL_TEXTURE_WRAP_T, type);
}




// ------- Parameter setting

template<typename T>
void Texture::SetParameter(GLenum _n, T _v)
{
	glTexParameteri(TextureType, _n, (UINT)_v);
}

template<>
void Texture::SetParameter<float>(GLenum _n, float _v)
{
	glTexParameterf(TextureType, _n, _v);
}
template<>
void Texture::SetParameter<unsigned int>(GLenum _n, unsigned int _v)
{
	glTexParameteri(TextureType, _n, _v);
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