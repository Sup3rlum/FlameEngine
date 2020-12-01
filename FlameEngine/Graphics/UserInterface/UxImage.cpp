#include "UxImage.h"



UxImage::UxImage(fVector2 position, fVector2 size, Texture* texture)
	:
	Position(position),
	Size(size),
	mTexture(texture)
{

	mImageShader = Shader::FromSource("./shaders/ux/uxFrame.vert", "./shaders/ux/uxFrame.frag");


	pxBorderRadius = 0.0f;


	fVector3 w = Color::White.rgb;

	mTexture = new Texture(1, 1, GL_RGB32F, GL_RGB, GL_FLOAT, false);
	mTexture->SetWrappingMode(TextureWrapping::REPEAT);
	mTexture->SetFilteringMode(TextureFiltering::BILINEAR);
	mTexture->SetData(&w);

}
void UxImage::Render()
{

	RenderState::Push(mRenderState);

	mImageShader->UseProgram();
	mImageShader->SetFloat("BorderRadius", pxBorderRadius);
	mImageShader->SetVector("Dimensions", Size);
	mImageShader->SetMatrix("View", GetParent()->pRenderingService->mView);
	mImageShader->SetMatrix("MatrixTransforms", fMatrix4::Translation(fVector3(Position, 0)) * fMatrix4::Scaling(fVector3(Size, 1)));
	mImageShader->SetTexture(0, mTexture);

	GetParent()->pRenderingService->DrawElement();

	RenderState::Pop();
}
void UxImage::Update()
{

}