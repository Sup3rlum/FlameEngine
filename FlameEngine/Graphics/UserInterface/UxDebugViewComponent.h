#pragma once

#include "../../dll/nchfx.h"
#include <intrin.h>


#include "UxLabel.h"

EXPORT_CLASS Scene;


EXPORT_CLASS UxDebugViewComponent : public UxComponent
{
public:

	UxDebugViewComponent(Scene* currentScene);


	void Render();
	void Update();

	void SetParent(UxContainer* p);

	Scene* currentScene;
	Shader* mDebugShader;
	UxLabel* DebugText;
	VertexBuffer* mVertexBuffer;


	STRING mCpuInfo;
	STRING mGpuInfo;
	STRING mMemoryInfo;

};

