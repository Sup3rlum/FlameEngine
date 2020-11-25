#include "UxDebugViewComponent.h"
#include "../../Scene/Scene.h"

UxDebugViewComponent::UxDebugViewComponent(Scene* currentScene)
{
	DebugText = new UxLabel(fVector2(0), "", new UxFont("C:\\Windows\\Fonts\\arial.ttf", 22));
	DebugText->TextColor = Color::White;
	DebugText->mMultiline = true;
	DebugText->Size = fVector2(600);

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


	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char CPUBrandString[0x40];
	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	mCpuInfo = STRING(CPUBrandString);
	mGpuInfo = STRING((char*)glGetString(GL_RENDERER));
	mMemoryInfo = to_string((statex.ullTotalPhys / 1024) / 1024) + " MB";

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

	DebugText->Update();

	int fps = (int)(1.0 / FrameTime::FrameDeltaTime.count());

	DebugText->Text = "FPS: " + std::to_string(fps) + "\n" +
		currentScene->CurrentCamera()->Position.ToString() + "\n\n" +
		"CPU: " + mCpuInfo + "\n"
		"GPU: " + mGpuInfo + "\n" +
		"Memory: " + mMemoryInfo + "\n";


	DebugText->Update();
}

void UxDebugViewComponent::SetParent(UxContainer* p)
{
	DebugText->SetParent(p);
	__super::SetParent(p);
}