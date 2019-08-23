#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(vector<VertexElements> vertexElements)
{
	_elements = vertexElements;



}



void VertexBuffer::Render(GLenum primType)
{
	glBindVertexArray(_vao);
	glDrawArrays(primType, 0, _primitiveCount);
}

void VertexBuffer::RenderIndexed(GLenum primType)
{
	glBindVertexArray(_vao);
	glDrawElements(primType, _indexCount, GL_UNSIGNED_INT, (void*)0);
}