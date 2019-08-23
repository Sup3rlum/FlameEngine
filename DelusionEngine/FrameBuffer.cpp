#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(GLuint _w, GLuint _h)
{
	_width = _w;
	_height = _h;

	glGenFramebuffers(1, &_handle);
}

void FrameBuffer::BindTexture(Texture* _tex, GLuint attachment)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, _tex->_handle, 0);
}
void FrameBuffer::SetAttachments(GLenum* _att, int count)
{
	glDrawBuffers(count, _att);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
}
void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}