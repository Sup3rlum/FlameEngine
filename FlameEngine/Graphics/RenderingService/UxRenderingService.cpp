#include "UxRenderingService.h"


VertexTexture mVertexData[4] =
{
	VertexTexture(FVector3(0,0,0), FVector2(0,1)),
	VertexTexture(FVector3(1,0,0), FVector2(1,1)),
	VertexTexture(FVector3(1,1,0), FVector2(1,0)),
	VertexTexture(FVector3(0,1,0), FVector2(0,0))
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

		mView = FMatrix4::CreateOrthographic(0.0f, (float)context->_contextDescription->width, (float)context->_contextDescription->height, 0.0f, 0.0f, 1.0f);

		Shader* shaders[2]
		{
			FLSLCompilerService::CompileShaderFromSourceFile("./shaders/renderbatch.vert", ShaderType::VERTEX),
			FLSLCompilerService::CompileShaderFromSourceFile("./shaders/renderbatch.frag", ShaderType::FRAGMENT)
		};


		mDefaultComponentShader = new Program(shaders);

		mVertexBuffer = new VertexBuffer(VertexTexture::Elements);

		mVertexBuffer->SetIndexedData<VertexTexture>(mVertexData, mIndexData, 4, 6);

	}
}


void UxRenderingService::SetView()
{
	SetView(mView);
}
void UxRenderingService::SetView(FMatrix4 matrix)
{
	mDefaultComponentShader->SetUniform("View", matrix);
}


void UxRenderingService::UseShader()
{
	mDefaultComponentShader->UseProgram();
}
void UxRenderingService::DrawElement()
{

	mVertexBuffer->RenderIndexed(GL_TRIANGLES);
}