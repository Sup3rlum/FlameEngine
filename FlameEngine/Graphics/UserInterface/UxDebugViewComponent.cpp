#include "UxDebugViewComponent.h"
#include "../../Scene/Scene.h"

UxDebugViewComponent::UxDebugViewComponent(Scene* currentScene)
{
	DebugText = new UxLabel(fVector2(0), "", new UxFont("C:\\Windows\\Fonts\\arial.ttf", 22));
	DebugText->TextColor = Color::White;
	DebugText->mMultiline = true;
	DebugText->Size = fVector2(600);
	//DebugText->BackgroundColor = Color(0, 0, 0, 130);

	this->currentScene = currentScene;

	VertexColor lVertexData[] =
	{
		VertexColor(fVector3(0,0,0),fVector3(1,0,0)),
		VertexColor(fVector3(1,0,0),fVector3(1,0,0)),

		VertexColor(fVector3(0,0,0),fVector3(0,1,0)),
		VertexColor(fVector3(0,1,0),fVector3(0,1,0)),

		VertexColor(fVector3(0,0,0),fVector3(0,0,1)),
		VertexColor(fVector3(0,0,1),fVector3(0,0,1)),

	};

	mDebugShader = Shader::FromSource("./shaders/debug_view.vert", "./shaders/debug_view.frag");
	mVertexBuffer = new VertexBuffer(VertexColor::Elements);


	mVertexBuffer->SetData(lVertexData, 6);

}

void UxDebugViewComponent::Render()
{

	DebugText->Render();

	RenderState::Push(mRenderState);

	mDebugShader->UseProgram();

	mDebugShader->SetMatrix("View",			currentScene->CurrentCamera()->DebugView);
	mDebugShader->SetMatrix("Projection",	currentScene->CurrentCamera()->Projection);
	mDebugShader->SetMatrix("World",		fMatrix4::Identity());

	mVertexBuffer->Render(GL_LINES);

	RenderState::Pop();
}
void UxDebugViewComponent::Update()
{


	int fps = (int)(1.0 / FrameTime::FrameDeltaTime.count());
	
	DebugText->Text =	"FPS: " + std::to_string(fps) + "\n" +
						Runtime::handlingInstance->sysInfo.cpuInfo.oemString + "\n" + 
						Runtime::handlingInstance->sysInfo.gpuInfo.oemString + "\n" +
						Runtime::handlingInstance->sysInfo.gpuInfo.driverString + "\n" +
						"Memory: "		+ to_string(Runtime::handlingInstance->sysInfo.memoryInfo.size / (1024 * 1024)) + "MB\n";


	DebugText->Update();
}

void UxDebugViewComponent::SetParent(UxContainer* p)
{
	DebugText->SetParent(p);
	__super::SetParent(p);
}