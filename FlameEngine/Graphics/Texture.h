#pragma once

#include "../dll/nchfx.h"
#include "../util/stb_image.h"

using namespace glm;

EXPORT_CLASS Texture
{
public:

	Texture(STRING filepath);
	Texture(GLuint with, GLuint height, GLint _colorFormat, GLint outerColorFormat, GLint dataFormat, bool mimmap);
	Texture(GLuint with, GLuint height, GLint _colorFormat, GLint outerColorFormat, GLint dataFormat, bool mimmap, int sampleCount);

	void SetParameter(GLenum name, GLenum param);
	void SetParameterf(GLenum name, float param);

	void Bind();
	void Unbind();

	template<typename T>
	void SetData(T* _data)
	{
		_dataInternal = _data;

		glBindTexture(GL_TEXTURE_2D, _handle);
		glTexImage2D(GL_TEXTURE_2D, 0, _colorFormat, _width, _height, 0, _outerColorFormat, _dataFormat, _dataInternal);
	}

	GLuint _handle;

	GLint _colorFormat;
	GLint _outerColorFormat;
	GLint _dataFormat;

	void* _dataInternal;

	int _width;
	int _height;
	int _channels;

	bool IsMultiSampled;

	GLenum TextureType;

	vec4 GetData(int w, int h);
};

