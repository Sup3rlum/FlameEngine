#include "RenderBatch.h"

VertexTexture _vert[4] =
{
	VertexTexture(fVector3(0,0,0), fVector2(0,0)),
	VertexTexture(fVector3(1,0,0), fVector2(1,0)),
	VertexTexture(fVector3(1,1,0), fVector2(1,1)),
	VertexTexture(fVector3(0,1,0), fVector2(0,1))
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


RenderBatch::RenderBatch(Context* _c)
{
	View = ortho(0.0f,(float)_c->_contextDescription->width,(float)_c->_contextDescription->height, 0.0f, 0.0f, 1.0f);

	_shader			= Shader::FromSource("./shaders/renderbatch.vert", "./shaders/renderbatch.frag");
	_shaderString	= Shader::FromSource("./shaders/renderbatch_string.vert", "./shaders/renderbatch_string.frag");

	_vb = new VertexBuffer(VertexTexture::Elements);

	_vb->SetIndexedData<VertexTexture>(_vert,_ind, 4, 6);

	State = new RenderState();
	
	State->CullState = CullState::Front;
	State->DepthFunction = DepthFunc::Always;

	State->SourceBlend = BlendFunc::SourceAlpha;
	State->DestinationBlend = BlendFunc::OneMinusSourceAlpha;


}


void RenderBatch::DrawTexture(Texture* _tex, float x, float y, float width, float height)
{
	RenderState::Push(State);

	_shader->UseProgram();

	_shader->SetMatrix("View", View);
	_shader->SetMatrix("MatrixTransforms", translate(identity<fMatrix4>(), fVector3(x, y, 0)) * scale(identity<fMatrix4>(), fVector3(width, height, 1)));
	_shader->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();
}

void RenderBatch::DrawTexture(Texture* _tex, float x, float y, float width, float height, Shader* _sh)
{
	RenderState::Push(State);

	_sh->SetMatrix("View", View);
	_sh->SetMatrix("MatrixTransforms", translate(identity<fMatrix4>(), fVector3(x, y, 0)) * scale(identity<fMatrix4>(), fVector3(width, height, 1)));
	_sh->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();
}


void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height)
{


	_shader->UseProgram();

	_shader->SetMatrix("View", View);
	_shader->SetMatrix("MatrixTransforms", translate(identity<fMatrix4>(), fVector3(x, y, 0)) * scale(identity<fMatrix4>(), fVector3(width, height, 1)));

	for (int i = 0; i < count; i++)
	{
		_shader->SetTexture(i, _tex[i]);
	}

	_vb->RenderIndexed(GL_TRIANGLES);
}

void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, Shader* _sh)
{
	RenderState::Push(State);

	_sh->SetMatrix("View", View);
	_sh->SetMatrix("MatrixTransforms", translate(identity<fMatrix4>(), fVector3(x, y, 0)) * scale(identity<fMatrix4>(), fVector3(width, height, 1)));

	for (int i = 0; i < count; i++)
	{
		_sh->SetTexture(i, _tex[i]);
	}

	_vb->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();
}

void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, fMatrix4 MatrixTransforms, Shader* _sh)
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

	RenderState::Push(State);

	_shaderString->UseProgram();

	_shaderString->SetVector("Color", color);
	_shaderString->SetMatrix("View", View);

	for (int c = 0; c < text.length(); c++)
	{
		Character ch = font->Characters[text[c]];

		_shaderString->SetMatrix(
			"MatrixTransforms",
			translate
			(
				identity<fMatrix4>(),
				fVector3(x + ch.Bearing.x, y - ch.Bearing.y, 0)
			) * scale
			(
				identity<fMatrix4>(),
				fVector3(ch.Size.x, ch.Size.y, 1)
			)
		);
		_shaderString->SetTexture(0, ch.texture);

		_vb->RenderIndexed(GL_TRIANGLES);

		x += (ch.Advance >> 6);
	}

	RenderState::Pop();
}