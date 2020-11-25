#include "UxRenderingService.h"


VertexTexture mVertexData[4] =
{
	VertexTexture(fVector3(0,0,0), fVector2(0,1)),
	VertexTexture(fVector3(1,0,0), fVector2(1,1)),
	VertexTexture(fVector3(1,1,0), fVector2(1,0)),
	VertexTexture(fVector3(0,1,0), fVector2(0,0))
};
UINT mIndexData[6]
{
	0,
	1,
	2,

	0,
	2,
	3
};


UxRenderingService::UxRenderingService(Context* context)
{
	if (context != NULL)
	{
		mAttachedContext = context;

		mView = fMatrix4::CreateOrthographic(0.0f, (float)context->_contextDescription->width, (float)context->_contextDescription->height, 0.0f, 0.0f, 1.0f);

		mDefaultComponentShader = Shader::FromSource("./shaders/renderbatch.vert", "./shaders/renderbatch.frag");

		mVertexBuffer = new VertexBuffer(VertexTexture::Elements);

		mVertexBuffer->SetIndexedData<VertexTexture>(mVertexData, mIndexData, 4, 6);

	}
}


void UxRenderingService::SetView()
{
	SetView(mView);
}
void UxRenderingService::SetView(fMatrix4 matrix)
{
	mDefaultComponentShader->SetMatrix("View", matrix);
}


void UxRenderingService::UseShader()
{
	mDefaultComponentShader->UseProgram();
}
void UxRenderingService::DrawElement()
{

	mVertexBuffer->RenderIndexed(GL_TRIANGLES);
}