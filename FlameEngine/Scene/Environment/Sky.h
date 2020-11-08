#pragma once


#include "../../dll/nchfx.h"

#include "../GameObjects/RenderableObject.h"
#include "../GameObjects/Model.h"


EXPORT_CLASS Scene;

EXPORT_CLASS Sky : public RenderableObject
{
public:

	Sky(Scene* _sc);

	void Render();
	Model _skyboxModel;

	Scene* _scene;

};