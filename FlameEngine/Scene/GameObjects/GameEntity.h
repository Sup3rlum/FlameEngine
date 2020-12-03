#pragma once

#include "Actor.h"
#include "../../FileSystem/AssetManager.h"



EXPORT(class,  GameEntity) : public Actor
{

public:
	GameEntity(STRING _modelName);

	void Render();
	void Update(FrameTime* _frameTime);

	StaticModel model;
};

