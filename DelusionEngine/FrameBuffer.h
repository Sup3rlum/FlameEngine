#pragma once

#include "nchfx.h"

#include "Texture.h"

using namespace std;

class DELUSION_DLL FrameBuffer
{
public:

	FrameBuffer(GLuint width, GLuint height);

	void BindTexture(Texture* _texture, GLenum attachment);
	void SetAttachments(GLenum* _attachments, int num);

	void Bind();
	void Unbind();

	GLuint _handle;

	GLuint _width;
	GLuint _height;

	vector<GLuint> attachments;
};

