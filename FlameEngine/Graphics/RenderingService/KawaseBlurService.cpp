#include "KawaseBlurService.h"


KawaseBlurService::KawaseBlurService(Context* context, bool halfRes) : RenderingServiceBase(context)
{

	mShader = Shader::FromSource(".\\shaders\\renderbatch.vert", ".\\shaders\\blur\\kawase_blur.frag");

	mBuffer = new Texture(2560, 1440, GL_RGBA32F, GL_RGBA, GL_FLOAT, true);
	mBuffer->SetFilteringMode(TextureFiltering::BILINEAR);
	mBuffer->SetWrappingMode(TextureWrapping::REPEAT);

	mFrameBuffer = new FrameBuffer(2560, 1440);
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

		mShader->SetFloat("PassIndex", 1.5f);
		mShader->SetMatrix("View", View);
		mShader->SetMatrix("MatrixTransforms", fMatrix4::Scaling(fVector3(mAttachedContext->_contextDescription->width, mAttachedContext->_contextDescription->height, 1)));

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

		mShader->SetFloat("PassIndex", 2.5f);
		mShader->SetMatrix("View", View);
		mShader->SetMatrix("MatrixTransforms", fMatrix4::Scaling(fVector3(mAttachedContext->_contextDescription->width, mAttachedContext->_contextDescription->height, 1)));

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

		mShader->SetFloat("PassIndex", 3.5f);
		mShader->SetMatrix("View", View);
		mShader->SetMatrix("MatrixTransforms", fMatrix4::Scaling(fVector3(mAttachedContext->_contextDescription->width, mAttachedContext->_contextDescription->height, 1)));

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

		mShader->SetFloat("PassIndex", 4.5f);
		mShader->SetMatrix("View", View);
		mShader->SetMatrix("MatrixTransforms", fMatrix4::Scaling(fVector3(mAttachedContext->_contextDescription->width, mAttachedContext->_contextDescription->height, 1)));

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

		mShader->SetFloat("PassIndex", 4.5f);
		mShader->SetMatrix("View", View);
		mShader->SetMatrix("MatrixTransforms", fMatrix4::Scaling(fVector3(mAttachedContext->_contextDescription->width, mAttachedContext->_contextDescription->height, 1)));

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

		mShader->SetFloat("PassIndex", 5.5f);
		mShader->SetMatrix("View", View);
		mShader->SetMatrix("MatrixTransforms", fMatrix4::Scaling(fVector3(mAttachedContext->_contextDescription->width, mAttachedContext->_contextDescription->height, 1)));

		mShader->SetTexture(0, mBuffer);

		mQuadBuffer->RenderIndexed(GL_TRIANGLES);

		RenderState::Pop();
	}
	frameBuffer->Unbind();

}