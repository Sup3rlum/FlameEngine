#pragma once

#include "../../dll/nchfx.h"
#include "../../util/stb_image.h"
#include "../../Mathematics/Module.h"
#include "TextureFiltering.h"

EXPORT(class,  MultisampleTexture)
{
public:

	MultisampleTexture(STRING filepath, int sampleCount);
	MultisampleTexture(GLuint with, GLuint height, GLint _colorFormat, int sampleCount);

	template<typename T>
	void SetParameter(GLenum name, T param);

	template<>
	void SetParameter<float>(GLenum name, float param);
	template<>
	void SetParameter<unsigned int>(GLenum name, unsigned int param);


	void Bind();
	void Unbind();

	template<typename T>
	void SetData(T* _data)
	{
	/*	Bind();

		_dataInternal = _data;
		glTexImage2D(GL_TEXTURE_2D, 0, _colorFormat, _width, _height, 0, _outerColorFormat, _dataFormat, _dataInternal);*/
	}


	void SetFilteringMode(TextureFiltering filteringType);
	void SetWrappingMode(TextureWrapping filteringType);


	TextureFiltering mFilteringType;
	TextureWrapping mWrappingMode;

	GLuint _handle;

	GLint _colorFormat;


	void* _dataInternal;

	int _width;
	int _height;
	int _channels;

	int samples;

	GLenum TextureType;

	fVector4 GetData(int w, int h);
};
