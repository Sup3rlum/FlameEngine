#pragma once

#include "../../dll/nchfx.h"
#include "../../util/stb_image.h"
#include "../../Mathematics/Module.h"
#include "TextureFiltering.h"



EXPORT(class, ITextureBase)
{
public:




	void SetParameter(GLenum name, float param);
	void SetParameter(GLenum name, int param);


	void Bind();
	void Unbind();

	virtual void SetData(void* _data);
	virtual ITextureBase* Clone();

	void SetFilteringMode(TextureFiltering filteringType);
	void SetWrappingMode(TextureWrapping filteringType);

	void ForceGenerateMipmaps();

	uint GetHandle();
	uint GetInternalType();

protected:
	GLenum TextureType;

	GLuint handle;

	GLint colorFormat;
	GLint outerColorFormat;
	GLint dataFormat;

	int channels;

	TextureFiltering mFilteringType;
	TextureWrapping mWrappingMode;
};

