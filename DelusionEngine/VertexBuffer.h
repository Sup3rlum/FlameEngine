#pragma once

#include "nchfx.h"

#include "VertexElements.h"

using namespace glm;
using namespace std;


class DELUSION_DLL VertexBuffer
{
	DELHandle _vao;
	DELHandle _vbo;
	DELHandle _ibo;
public:
	VertexBuffer(vector<VertexElements> elements);

	DELuint32 _primitiveCount;
	DELuint32 _indexCount;

	vector<VertexElements> _elements;

	template <typename T>
	void SetData(T* _data, int count)
	{

		_primitiveCount = count;

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);



		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T) * count, _data, GL_STATIC_DRAW);


		for (int i = 0; i < _elements.size(); i++)
		{

			glVertexAttribPointer
			(
				_elements[i]._atribNumber,
				_elements[i]._size,
				_elements[i]._type,
				_elements[i]._normalized,
				_elements[i]._stride,
				_elements[i]._offset
			);
			glEnableVertexAttribArray(_elements[i]._atribNumber);
		}
	};

	template <typename K>
	void SetIndexedData(K* _data, GLuint* _indices, int primCount, int indexCount)
	{
		_primitiveCount = primCount;
		_indexCount = indexCount;

		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);



		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(K) * primCount, _data, GL_STATIC_DRAW);


		for (int i = 0; i < _elements.size(); i++)
		{

			glVertexAttribPointer
			(
				_elements[i]._atribNumber,
				_elements[i]._size,
				_elements[i]._type,
				_elements[i]._normalized,
				_elements[i]._stride,
				_elements[i]._offset
			);
			glEnableVertexAttribArray(_elements[i]._atribNumber);
		}

		glGenBuffers(1, &_ibo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, _indices, GL_STATIC_DRAW);


		GLint size = 0;

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	};

	void Render(GLenum type);
	void RenderIndexed(GLenum type);

};


