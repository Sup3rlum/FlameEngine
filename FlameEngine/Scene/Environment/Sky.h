#pragma once


#include "../../dll/nchfx.h"

#include "../GameObjects/RenderableObject.h"
#include "../../Graphics/ModelSystem/ModelBase.h"


EXPORT_FORWARD_DECL(class,  Scene);

EXPORT(class,  Sky) : public RenderableObject
{
public:

	Sky(Scene* _sc);

	void Render();
	ModelBase _skyboxModel;

	Scene* _scene;

};