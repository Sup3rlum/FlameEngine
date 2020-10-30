#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(vector<VertexElements> vertexElements)
{
	_elements = vertexElements;

}
VertexBuffer::VertexBuffer()
{

}



void VertexBuffer::Render(GLenum primType)
{
	glBindVertexArray(_vao);
	glDrawArrays(primType, 0, _primitiveCount);
	glBindVertexArray(0);
}

void VertexBuffer::RenderIndexed(GLenum primType)
{

	glBindVertexArray(_vao);
	glDrawElements(primType, _indexCount, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}