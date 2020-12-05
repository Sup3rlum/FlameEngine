#include "MultisampleTexture.h"


MultisampleTexture::MultisampleTexture(STRING _path, int sampleCount)
{
	samples = sampleCount;

	TextureType = GL_TEXTURE_2D_MULTISAMPLE;

	glGenTextures(1, &_handle);
	glBindTexture(TextureType, _handle);

	SetWrappingMode(TextureWrapping::REPEAT);
	SetFilteringMode(TextureFiltering::NONE);


	_dataInternal = stbi_load(_path.c_str(), &_width, &_height, &_channels, 0);

	//glTexImage2DMultisample(TextureType, samples, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _dataInternal);
	glGenerateMipmap(TextureType);
}

MultisampleTexture::MultisampleTexture(GLuint width, GLuint height, GLint colorFormat, int sampleCount)
{
	samples = sampleCount;

	TextureType = GL_TEXTURE_2D_MULTISAMPLE;

	_width = width;
	_height = height;

	_colorFormat = colorFormat;


	glGenTextures(1, &_handle);
	glBindTexture(TextureType, _handle);

	glTexImage2DMultisample(TextureType, samples, _colorFormat, width, height, false);
	glGenerateMipmap(TextureType);
	
}


// ------ Filtering


void MultisampleTexture::SetFilteringMode(TextureFiltering type)
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

void MultisampleTexture::SetWrappingMode(TextureWrapping type)
{
	mWrappingMode = type;

	Bind();

	SetParameter(GL_TEXTURE_WRAP_S, type);
	SetParameter(GL_TEXTURE_WRAP_T, type);
}




// ------- Parameter setting

template<typename T>
void MultisampleTexture::SetParameter(GLenum _n, T _v)
{
	glTexParameteri(TextureType, _n, (UINT)_v);
}

template<>
void MultisampleTexture::SetParameter<float>(GLenum _n, float _v)
{
	glTexParameterf(TextureType, _n, _v);
}
template<>
void MultisampleTexture::SetParameter<unsigned int>(GLenum _n, unsigned int _v)
{
	glTexParameteri(TextureType, _n, _v);
}





void MultisampleTexture::Bind()
{
	glBindTexture(TextureType, _handle);
}
void MultisampleTexture::Unbind()
{
	glBindTexture(TextureType, 0);
}


FVector4 MultisampleTexture::GetData(int x, int y)
{
	unsigned char* _offs = (unsigned char*)_dataInternal + (x + _height * y) * 4;

	return FVector4
	(
		_offs[0],
		_offs[1],
		_offs[2],
		_offs[3]
	);
}