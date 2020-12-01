#include "BoundingVolumeDebugService.h"


BoundingVolumeDebugService::BoundingVolumeDebugService(Context* context) : RenderingServiceBase(context)
{

	mVertexBuffer = new VertexBuffer(VertexColor::Elements);

	mDebugShader = Shader::FromSource("./shaders/debug_view.vert", "./shaders/debug_view.frag");

}

void BoundingVolumeDebugService::Render(Camera* cam, fVector3* corners)
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

	mDebugShader->SetMatrix("View", cam->View);
	mDebugShader->SetMatrix("Projection", cam->Projection);
	mDebugShader->SetMatrix("World", fMatrix4::Identity());


	mVertexBuffer->Render(GL_LINES);

	RenderState::Pop();
}