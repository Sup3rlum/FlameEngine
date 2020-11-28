#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(GLuint _w, GLuint _h)
{
	_width = _w;
	_height = _h;

	glGenFramebuffers(1, &_handle);
}

void FrameBuffer::BindTexture(Texture* _tex, GLuint attachment)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, _tex->TextureType, _tex->_handle, 0);
}

void FrameBuffer::BindTexture(MultisampleTexture* _tex, GLuint attachment)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, _tex->TextureType, _tex->_handle, 0);
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

void FrameBuffer::EnableDepth()
{

	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 2560, 1440);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
}