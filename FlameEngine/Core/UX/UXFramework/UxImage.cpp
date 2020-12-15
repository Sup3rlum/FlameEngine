#include "UxImage.h"



UxImage::UxImage(FVector2 position, FVector2 size, Texture* texture)
	:
	Position(position),
	Size(size),
	mTexture(texture)
{


	Shader* shaders[2]
	{
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/ux/uxFrame.vert", ShaderType::VERTEX),
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/ux/uxFrame.frag", ShaderType::FRAGMENT)
	};


	mImageShader = new Program(shaders);



	pxBorderRadius = 0.0f;


	FVector3 w = Color::White.rgb;

	mTexture = new Texture(1, 1, GL_RGB32F, GL_RGB, GL_FLOAT);
	mTexture->SetWrappingMode(TextureWrapping::REPEAT);
	mTexture->SetFilteringMode(TextureFiltering::BILINEAR);
	mTexture->SetData(&w);

}
void UxImage::Render()
{

	RenderState::Push(mRenderState);

	mImageShader->UseProgram();
	mImageShader->SetUniform("BorderRadius", pxBorderRadius);
	mImageShader->SetUniform("Dimensions", Size);
	mImageShader->SetUniform("View", GetParent()->pRenderingService->mView);
	mImageShader->SetUniform("MatrixTransforms", FMatrix4::Translation(FVector3(Position, 0)) * FMatrix4::Scaling(FVector3(Size, 1)));

	mImageShader->SetTexture(0, mTexture);

	GetParent()->pRenderingService->DrawElement();

	RenderState::Pop();
}
void UxImage::Update()
{

}