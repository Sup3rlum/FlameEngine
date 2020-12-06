#include "RenderBatch.h"

VertexTexture _vert[4] =
{
	VertexTexture(FVector3(0,0,0), FVector2(0,1)),
	VertexTexture(FVector3(1,0,0), FVector2(1,1)),
	VertexTexture(FVector3(1,1,0), FVector2(1,0)),
	VertexTexture(FVector3(0,1,0), FVector2(0,0))
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
	View = FMatrix4::CreateOrthographic(0.0f,(float)_c->_contextDescription->width,(float)_c->_contextDescription->height, 0.0f, 0.0f, 1.0f);

	Shader* shaders[2] =
	{
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/renderbatch.vert", ShaderType::VERTEX),
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/renderbatch.frag", ShaderType::FRAGMENT)
	};
	Shader* stringshaders[2] =
	{
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/renderbatch_string.vert", ShaderType::VERTEX),
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/renderbatch_string.frag", ShaderType::FRAGMENT)
	};



	_shader = new Program(shaders);
	_shaderString = new Program(stringshaders);


	_vb = new VertexBuffer(VertexTexture::Elements);

	_vb->SetIndexedData<VertexTexture>(_vert,_ind, 4, 6);

	State = new RenderState();
	
	State->CullState = CullState::Front;
	State->DepthFunction = DepthFunc::Always;

	State->SourceBlend = BlendFunc::SourceAlpha;
	State->DestinationBlend = BlendFunc::OneMinusSourceAlpha;


}
// MUTLISAMPLE TEXTURE DRAWING



void RenderBatch::DrawMultisampleTexture(MultisampleTexture* _tex, float x, float y, float width, float height)
{
	RenderState::Push(State);

	_shader->UseProgram();

	_shader->SetUniform("View", View);
	_shader->SetUniform("MatrixTransforms", FMatrix4::Translation(FVector3(x, y, 0)) * FMatrix4::Scaling(FVector3(width, height, 1)));
	_shader->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();
}




// TEXTURE DRAWING

void RenderBatch::DrawTexture(Texture* _tex, float x, float y, float width, float height)
{
	RenderState::Push(State);

	_shader->UseProgram();

	_shader->SetUniform("View", View);
	_shader->SetUniform("MatrixTransforms", FMatrix4::Translation(FVector3(x, y, 0)) * FMatrix4::Scaling(FVector3(width, height, 1)));
	_shader->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();
}

void RenderBatch::DrawTexture(Texture* _tex, float x, float y, float width, float height, Program* _sh)
{
	RenderState::Push(State);

	_sh->SetUniform("View", View);
	_sh->SetUniform("MatrixTransforms", FMatrix4::Translation(FVector3(x, y, 0)) * FMatrix4::Scaling(FVector3(width, height, 1)));
	_sh->SetTexture(0, _tex);

	_vb->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();
}


void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height)
{


	_shader->UseProgram();

	_shader->SetUniform("View", View);
	_shader->SetUniform("MatrixTransforms", FMatrix4::Translation(FVector3(x, y, 0)) * FMatrix4::Scaling(FVector3(width, height, 1)));

	for (int i = 0; i < count; i++)
	{
		_shader->SetTexture(i, _tex[i]);
	}

	_vb->RenderIndexed(GL_TRIANGLES);
}

void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, Program* _sh)
{
	RenderState::Push(State);

	_sh->SetUniform("View", View);
	_sh->SetUniform("MatrixTransforms", FMatrix4::Translation(FVector3(x, y, 0)) * FMatrix4::Scaling(FVector3(width, height, 1)));

	for (int i = 0; i < count; i++)
	{
		_sh->SetTexture(i, _tex[i]);
	}

	_vb->RenderIndexed(GL_TRIANGLES);

	RenderState::Pop();
}

void RenderBatch::DrawTextures(int count, Texture** _tex, float x, float y, float width, float height, FMatrix4 MatrixTransforms, Program* _sh)
{
	for (int i = 0; i < count; i++)
	{
		_sh->SetUniform("MatrixTransforms", MatrixTransforms);
		_sh->SetTexture(i, _tex[i]);
	}

	_vb->RenderIndexed(GL_TRIANGLES);
}



void RenderBatch::DrawString(string text, UxFont* font, float x, float y, Color color)
{

	RenderState::Push(State);

	_shaderString->UseProgram();

	_shaderString->SetUniform("Color", color);
	_shaderString->SetUniform("View", View);

	for (int c = 0; c < text.length(); c++)
	{
		UxCharacter ch = font->Characters[text[c]];

		_shaderString->SetUniform(
			"MatrixTransforms",
			FMatrix4::Translation
			(

				FVector3(x + ch.Bearing.x, y - ch.Bearing.y, 0)
			) 
			* 
			FMatrix4::Scaling
			(
				FVector3(ch.Size.x, ch.Size.y, 1)
			)
		);
		_shaderString->SetTexture(0, ch.texture);

		_vb->RenderIndexed(GL_TRIANGLES);

		x += (ch.Advance >> 6);
	}

	RenderState::Pop();
}