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


RenderBatch::RenderBatch(BaseContext* _c)
{
	View = ortho(0.0f,(float)_c->_contextDescription.width,(float)_c->_contextDescription.height, 0.0f, 0.0f, 1.0f);

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


void RenderBatch::DrawString(string text, Font* font, float x, float y, Color color)
{

	_shaderString->UseProgram();

	_shaderString->SetVector("Color", color);
	_shaderString->SetMatrix("View", View);

	for (int c=0;c<text.length();c++)
	{
		Character ch = font->Characters[text[c]];

		_shaderString->SetMatrix(
			"MatrixTransforms",
			translate
			(
				identity<Matrix4>(),
				Vector3(x + ch.Bearing.x, y - ch.Bearing.y, 0)
			) * scale
			(
				identity<Matrix4>(),
				Vector3(ch.Size.x, ch.Size.y, 1)
			)
		);
		_shaderString->SetTexture(0, ch.texture);

		_vb->RenderIndexed(GL_TRIANGLES);

		x += (ch.Advance >> 6);
	}

}