#include "RenderBatch.h"

VertexTexture _vert[4] =
{
	VertexTexture(Vector3(0,0,0), Vector2(0,0)),
	VertexTexture(Vector3(1,0,0), Vector2(1,0)),
	VertexTexture(Vector3(1,1,0), Vector2(1,1)),
	VertexTexture(Vector3(0,1,0), Vector2(0,1))
};
GLuint _ind[6]
{
	0,
	1,
	2,

	0,
	2,
	3
};


RenderBatch::RenderBatch(ContextDescription* _c)
{
	View = ortho(0.0f,(float)_c->width,(float)_c->height, 0.0f, 0.0f, 1.0f);

	_shader = new Shader("./shaders/renderbatch.vert", "./shaders/renderbatch.frag");
	_shaderString = new Shader("./shaders/renderbatch_string.vert", "./shaders/renderbatch_string.frag");

	_vb = new VertexBuffer(VertexTexture::Elements);

	_vb->SetIndexedData<VertexTexture>(_vert,_ind, 4, 6);
}


void RenderBatch::DrawTexture(Texture* _tex, float x, float y, float width, float height)
{
	

	_shader->UseProgram();

	_shader->SetMatrix("View", View);
	_shader->SetMatrix("MatrixTransforms", translate(identity<Matrix4>(), Vector3(x, y, 0)) * scale(identity<Matrix4>(), Vector3(width, height, 1)));
	_shader->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);
}

void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height)
{


	_shader->UseProgram();

	_shader->SetMatrix("View", View);
	_shader->SetMatrix("MatrixTransforms", translate(identity<Matrix4>(), Vector3(x, y, 0)) * scale(identity<Matrix4>(), Vector3(width, height, 1)));

	for (int i = 0; i < count; i++)
	{
		_shader->SetTexture(i, _tex[i]);
	}

	_vb->RenderIndexed(GL_TRIANGLES);
}

void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, Matrix4 MatrixTransforms, Shader* _sh)
{
	for (int i = 0; i < count; i++)
	{
		_sh->SetMatrix("MatrixTransforms", MatrixTransforms);
		_sh->SetTexture(i, _tex[i]);
	}

	_vb->RenderIndexed(GL_TRIANGLES);
}


void RenderBatch::DrawTexture(GLuint _tex, float x, float y, float width, float height)
{

	_shader->UseProgram();

	_shader->SetMatrix("View", View);
	_shader->SetMatrix("MatrixTransforms", translate(identity<Matrix4>(), Vector3(x, y, 0)) * scale(identity<Matrix4>(), Vector3(width, height, 1)));
	_shader->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);
}
void RenderBatch::DrawString(string text, Font* font, float x, float y, Color color)
{
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = font->Characters[*c];

		GLfloat w = ch.Size.x;
		GLfloat h = ch.Size.y;


		GLfloat xpos = x + ch.Bearing.x;
		GLfloat ypos = y - ch.Bearing.y;


		//DrawTexture(ch.texture, xpos, ypos, w, h);

		_shaderString->UseProgram();

		_shaderString->SetVector("Color", color);
		_shaderString->SetMatrix("View", View);
		_shaderString->SetMatrix("MatrixTransforms", translate(identity<Matrix4>(), Vector3(xpos, ypos, 0)) * scale(identity<Matrix4>(), Vector3(w, h, 1)));
		_shaderString->SetTexture(0, ch.texture);

		_vb->RenderIndexed(GL_TRIANGLES);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

}