#include "ITextureBase.h"



void ITextureBase::SetFilteringMode(TextureFiltering type)
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
	}
}
void ITextureBase::SetAnisotropyLevel(float level)
{
	SetParameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
}

void ITextureBase::SetWrappingMode(TextureWrapping type)
{
	mWrappingMode = type;

	Bind();

	SetParameter(GL_TEXTURE_WRAP_S, (int)type);
	SetParameter(GL_TEXTURE_WRAP_T, (int)type);
}




// ------- Parameter setting


void ITextureBase::SetParameter(GLenum _n, float _v)
{
	glTexParameterf(TextureType, _n, _v);
}

void ITextureBase::SetParameter(GLenum _n, int _v)
{
	glTexParameteri(TextureType, _n, _v);
}



void ITextureBase::Bind()
{
	glBindTexture(TextureType, handle);
}
void ITextureBase::Unbind()
{
	glBindTexture(TextureType, 0);
}

void ITextureBase::ForceGenerateMipmaps()
{
	Bind();
	glGenerateMipmap(TextureType);
}

uint ITextureBase::GetHandle()
{
	return handle;
}
uint ITextureBase::GetInternalType ()
{
	return TextureType;
}


void ITextureBase::SetData(void* data)
{

}

ITextureBase* ITextureBase::Clone()
{
	return this;
}