#pragma once

#include "Model.h"
#include "Actor.h"
#include "../../IO/AssetManager.h"



EXPORT_CLASS GameEntity : public Actor
{

public:
	GameEntity(STRING _modelName);

	void Render();
	void RenderDepth();

	Model model;
};

