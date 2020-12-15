#pragma once

#include "../../dll/nchfx.h"

#include "Texture.h"
#include "MultisampleTexture.h"
#include "Color.h"
#include "../Renderer/OpenGL.h"

using namespace std;

EXPORT(class, FrameBuffer)
{
public:

	FrameBuffer(GLuint width, GLuint height);

	void BindTexture(Texture* _texture, GLenum attachment);
	void BindTexture(MultisampleTexture* _texture, GLenum attachment);
	void SetAttachments(GLenum* _attachments, int num);

	void Bind();
	void Unbind();

	void EnableDepth();
	void Clear(Color color);

	GLuint _handle;

	GLuint _width;
	GLuint _height;

	GLuint depthrenderbuffer;

	vector<GLuint> attachments;
};

