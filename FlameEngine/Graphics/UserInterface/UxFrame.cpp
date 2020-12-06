#include "UxFrame.h"



UxFrame::UxFrame(FVector2 position, FVector2 size) 
	: 
	Position(position), 
	Size(size)
{



	Shader* shaders[2]
	{
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/ux/uxFrame.vert", ShaderType::VERTEX),
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/ux/uxFrame.frag", ShaderType::FRAGMENT)
	};


	mFrameShader = new Program(shaders);


	color = Color::White;
	pxBorderRadius = 0.0f;


	FVector3 w = Color::White.rgb;

	mTexture = new Texture(1, 1, GL_RGB32F, GL_RGB, GL_FLOAT, false);
	mTexture->Bind();
	{
		mTexture->SetData(&w);

		mTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		mTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		mTexture->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		mTexture->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	mTexture->Unbind();
}

void UxFrame::Render()
{

	RenderState::Push(mRenderState);

	mFrameShader->UseProgram();

	mFrameShader->SetUniform("BorderRadius", pxBorderRadius);
	mFrameShader->SetUniform("Color", color);
	mFrameShader->SetUniform("Dimensions", Size);
	mFrameShader->SetUniform("View", GetParent()->pRenderingService->mView);
	mFrameShader->SetUniform("MatrixTransforms", FMatrix4::Translation(FVector3(Position, 0)) * FMatrix4::Scaling(FVector3(Size, 1)));

	mFrameShader->SetTexture(0, mTexture);

	GetParent()->pRenderingService->DrawElement();

	RenderState::Pop();
}

void UxFrame::Update()
{

}