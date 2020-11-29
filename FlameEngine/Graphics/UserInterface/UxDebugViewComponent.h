#pragma once

#include "../../dll/nchfx.h"
#include <intrin.h>


#include "UxLabel.h"

EXPORT_FORWARD_DECL(class,  Scene);


EXPORT(class,  UxDebugViewComponent) : public UxComponent
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

};
