#pragma once

#include "../../dll/nchfx.h"
#include "../../util/stb_image.h"
#include "../../Mathematics/Module.h"
#include "TextureFiltering.h"

EXPORT(class,  Texture)
{
public:

	Texture(STRING filepath);
	Texture(GLuint with, GLuint height, GLint _colorFormat, GLint outerColorFormat, GLint dataFormat, bool mimmap);


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
		Bind();

		_dataInternal = _data;
		glTexImage2D(TextureType, 0, _colorFormat, _width, _height, 0, _outerColorFormat, _dataFormat, _dataInternal);
	}


	void SetFilteringMode(TextureFiltering filteringType);
	void SetWrappingMode(TextureWrapping filteringType);


	TextureFiltering mFilteringType;
	TextureWrapping mWrappingMode;

	GLuint _handle;

	GLint _colorFormat;
	GLint _outerColorFormat;
	GLint _dataFormat;

	void* _dataInternal;

	int _width;
	int _height;
	int _channels;


	GLenum TextureType;

	fVector4 GetData(int w, int h);
};

