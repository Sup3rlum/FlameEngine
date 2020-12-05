#include "UxFrame.h"



UxFrame::UxFrame(FVector2 position, FVector2 size) 
	: 
	Position(position), 
	Size(size)
{

	mFrameShader = Shader::FromSource("./shaders/ux/uxFrame.vert", "./shaders/ux/uxFrame.frag");


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

	mFrameShader->SetFloat("BorderRadius", pxBorderRadius);
	mFrameShader->SetVector("Color", color);
	mFrameShader->SetVector("Dimensions", Size);
	mFrameShader->SetMatrix("View", GetParent()->pRenderingService->mView);
	mFrameShader->SetMatrix("MatrixTransforms", FMatrix4::Translation(FVector3(Position, 0)) * FMatrix4::Scaling(FVector3(Size, 1)));
	mFrameShader->SetTexture(0, mTexture);

	GetParent()->pRenderingService->DrawElement();

	RenderState::Pop();
}

void UxFrame::Update()
{

}