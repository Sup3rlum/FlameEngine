#pragma once

#include "nchfx.h"


typedef struct DELUSION_DLL VertexElements
{
	int _atribNumber;
	GLenum _type;
	GLboolean _normalized;
	GLsizei _stride;
	GLsizei _size;
	void* _offset;

	VertexElements(int att, GLsizei size, GLenum type, GLboolean norm, GLsizei str, void* offset)
	{
		_atribNumber = att;
		_type = type;
		_normalized = norm;
		_stride = str;
		_offset = offset;
		_size = size;
	}
};

