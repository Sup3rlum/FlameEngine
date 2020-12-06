#include "BoundingVolumeDebugService.h"


BoundingVolumeDebugService::BoundingVolumeDebugService(Context* context) : RenderingServiceBase(context)
{

	mVertexBuffer = new VertexBuffer(VertexColor::Elements);


	Shader* shaders[2] =
	{
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/debug_view.vert", ShaderType::VERTEX),
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/debug_view.frag", ShaderType::FRAGMENT)
	};


	mDebugShader = new Program(shaders);

}

void BoundingVolumeDebugService::Render(Camera* cam, FVector3* corners)
{


	VertexColor vData[24] = {
		VertexColor(corners[0], Color::Blue.rgb),
		VertexColor(corners[1], Color::Blue.rgb),
									 
		VertexColor(corners[1], Color::Blue.rgb),
		VertexColor(corners[2], Color::Blue.rgb),
									
		VertexColor(corners[2], Color::Blue.rgb),
		VertexColor(corners[3], Color::Blue.rgb),
									  
		VertexColor(corners[3], Color::Blue.rgb),
		VertexColor(corners[0], Color::Blue.rgb),



		VertexColor(corners[4], Color::Red.rgb),
		VertexColor(corners[5], Color::Red.rgb),
									   
		VertexColor(corners[5], Color::Red.rgb),
		VertexColor(corners[6], Color::Red.rgb),
									   
		VertexColor(corners[6], Color::Red.rgb),
		VertexColor(corners[7], Color::Red.rgb),
									   
		VertexColor(corners[7], Color::Red.rgb),
		VertexColor(corners[4], Color::Red.rgb),



		VertexColor(corners[0], Color::Black.rgb),
		VertexColor(corners[4], Color::Black.rgb),

		VertexColor(corners[1], Color::Black.rgb),
		VertexColor(corners[5], Color::Black.rgb),

		VertexColor(corners[2], Color::Black.rgb),
		VertexColor(corners[6], Color::Black.rgb),

		VertexColor(corners[3], Color::Black.rgb),
		VertexColor(corners[7], Color::Black.rgb)
	};

	mVertexBuffer->SetData(vData, 24);

	RenderState::Push(mRenderState);

	mDebugShader->UseProgram();

	mDebugShader->SetUniform("View", cam->View);
	mDebugShader->SetUniform("Projection", cam->Projection);
	mDebugShader->SetUniform("World", FMatrix4::Identity());


	mVertexBuffer->Render(GL_LINES);

	RenderState::Pop();
}