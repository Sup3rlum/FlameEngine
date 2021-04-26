#include "UxDebugViewComponent.h"

/*
#include "../../Scene/Scene.h"

UxDebugViewComponent::UxDebugViewComponent(Scene* currentScene)
{
	DebugText = new UxLabel(FVector2(0), "", new UxFont("C:\\Windows\\Fonts\\arial.ttf", 22));
	DebugText->TextColor = Color::White;
	DebugText->mMultiline = true;
	DebugText->Size = FVector2(600);
	//DebugText->BackgroundColor = Color(0, 0, 0, 130);

	this->currentScene = currentScene;

	VertexColor lVertexData[] =
	{
		VertexColor(FVector3(0,0,0),FVector3(1,0,0)),
		VertexColor(FVector3(1,0,0),FVector3(1,0,0)),

		VertexColor(FVector3(0,0,0),FVector3(0,1,0)),
		VertexColor(FVector3(0,1,0),FVector3(0,1,0)),

		VertexColor(FVector3(0,0,0),FVector3(0,0,1)),
		VertexColor(FVector3(0,0,1),FVector3(0,0,1)),

	};


	Shader* debugshaders[2]
	{
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/debug_view.vert", ShaderType::VERTEX),
		FLSLCompilerService::CompileShaderFromSourceFile("./shaders/debug_view.frag", ShaderType::FRAGMENT)
	};


	mDebugShader = new Program(debugshaders);
	mVertexBuffer = new VertexBuffer(VertexColor::Elements);


	mVertexBuffer->SetData(lVertexData, 6);

}

void UxDebugViewComponent::Render()
{

	DebugText->Render();

	RenderState::Push(mRenderState);

	mDebugShader->UseProgram();

	mDebugShader->SetUniform("View",			currentScene->CurrentCamera()->DebugView);
	mDebugShader->SetUniform("Projection",	currentScene->CurrentCamera()->Projection);
	mDebugShader->SetUniform("World",		FMatrix4::Identity());

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
						"Memory: "		+ to_string(Runtime::handlingInstance->sysInfo.memoryInfo.size / (1024 * 1024)) + "MB\n" +
						"Position: " + currentScene->CurrentCamera()->Position.ToString() + "\n" + 
						"Look: " + currentScene->CurrentCamera()->LookDirection.ToString() + "\n";


	DebugText->Update();
}

void UxDebugViewComponent::SetParent(UxContainer* p)
{
	DebugText->SetParent(p);
	__super::SetParent(p);
}*/