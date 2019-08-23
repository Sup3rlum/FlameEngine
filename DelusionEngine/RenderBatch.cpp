#include "RenderBatch.h"

VertexTexture _vert[4] =
{
	VertexTexture(vec3(0,0,0), vec2(0,1)),
	VertexTexture(vec3(1,0,0), vec2(1,1)),
	VertexTexture(vec3(1,1,0), vec2(1,0)),
	VertexTexture(vec3(0,1,0), vec2(0,0))
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


RenderBatch::RenderBatch(ContextParameters* _c)
{
	View = ortho(0.0f,(float)_c->width,(float)_c->height, 0.0f, 0.0f, 1.0f);

	_shader = new Shader("./shaders/renderbatch.vert", "./shaders/renderbatch.frag");

	_vb = new VertexBuffer(VertexTexture::Elements);

	_vb->SetIndexedData<VertexTexture>(_vert,_ind, 4, 6);
}


void RenderBatch::DrawTexture(Texture* _tex, float x, float y, float width, float height)
{
	

	_shader->UseProgram();

	_shader->SetMatrix("View", View);
	_shader->SetMatrix("MatrixTransforms", translate(identity<mat4x4>(), vec3(x, y, 0)) * scale(identity<mat4x4>(), vec3(width, height, 1)));
	_shader->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);
}

void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height)
{


	_shader->UseProgram();

	_shader->SetMatrix("View", View);
	_shader->SetMatrix("MatrixTransforms", translate(identity<mat4x4>(), vec3(x, y, 0)) * scale(identity<mat4x4>(), vec3(width, height, 1)));

	for (int i = 0; i < count; i++)
	{
		_shader->SetTexture(i, _tex[i]);
	}

	_vb->RenderIndexed(GL_TRIANGLES);
}

void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, mat4x4 MatrixTransforms, Shader* _sh)
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
	_shader->SetMatrix("MatrixTransforms", translate(identity<mat4x4>(), vec3(x, y, 0)) * scale(identity<mat4x4>(), vec3(width, height, 1)));
	_shader->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);
}