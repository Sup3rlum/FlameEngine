#include "KawaseBlurService.h"


KawaseBlurService::KawaseBlurService(Context* context, bool halfRes) : RenderingServiceBase(context)
{

	Shader* kawaseshaders[2] =
	{
		FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\renderbatch.vert", ShaderType::VERTEX),
		FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\blur\\kawase_blur.frag", ShaderType::FRAGMENT)
	};

	mShader = new Program(kawaseshaders);

	mBuffer = new Texture(4096, 4096, GL_RGBA32F, GL_RGBA, GL_FLOAT);
	mBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mFrameBuffer = new FrameBuffer(4096, 4096);
	mFrameBuffer->Bind();
	mFrameBuffer->BindTexture(mBuffer, GL_COLOR_ATTACHMENT0);
	mFrameBuffer->Unbind();


}

void KawaseBlurService::ApplyFilter(FrameBuffer* frameBuffer, Texture* texture, int passIndex)
{
	if (passIndex == 0)
		return;

	mFrameBuffer->Bind();
	{

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderState::Push(mRenderState);

		mShader->UseProgram();

		mShader->SetUniform("PassIndex", 1.5f);
		mShader->SetUniform("View", View);
		mShader->SetUniform("MatrixTransforms", FMatrix4::Identity());

		mShader->SetTexture(0, texture);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	mFrameBuffer->Unbind();


	frameBuffer->Bind();
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderState::Push(mRenderState);

		mShader->UseProgram();

		mShader->SetUniform("PassIndex", 2.5f);
		mShader->SetUniform("View", View);
		mShader->SetUniform("MatrixTransforms", FMatrix4::Identity());

		mShader->SetTexture(0, mBuffer);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	frameBuffer->Unbind();

	if (passIndex <= 1)
		return;

	mFrameBuffer->Bind();
	{

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderState::Push(mRenderState);

		mShader->UseProgram();

		mShader->SetUniform("PassIndex", 3.5f);
		mShader->SetUniform("View", View);
		mShader->SetUniform("MatrixTransforms", FMatrix4::Identity());

		mShader->SetTexture(0, texture);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	mFrameBuffer->Unbind();


	frameBuffer->Bind();
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderState::Push(mRenderState);

		mShader->UseProgram();

		mShader->SetUniform("PassIndex", 4.5f);
		mShader->SetUniform("View", View);
		mShader->SetUniform("MatrixTransforms", FMatrix4::Identity());

		mShader->SetTexture(0, mBuffer);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	frameBuffer->Unbind();


	if (passIndex <= 2)
		return;

	mFrameBuffer->Bind();
	{

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderState::Push(mRenderState);

		mShader->UseProgram();

		mShader->SetUniform("PassIndex", 4.5f);
		mShader->SetUniform("View", View);
		mShader->SetUniform("MatrixTransforms", FMatrix4::Identity());

		mShader->SetTexture(0, texture);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	mFrameBuffer->Unbind();


	frameBuffer->Bind();
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderState::Push(mRenderState);

		mShader->UseProgram();

		mShader->SetUniform("PassIndex", 5.5f);
		mShader->SetUniform("View", View);
		mShader->SetUniform("MatrixTransforms", FMatrix4::Identity());

		mShader->SetTexture(0, mBuffer);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	frameBuffer->Unbind();

}