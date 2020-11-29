#pragma once

#include "../../Graphics/ModelSystem/ModelBase.h"
#include "Actor.h"
#include "../../FileSystem/AssetManager.h"



EXPORT(class,  GameEntity) : public Actor
{

public:
	GameEntity(STRING _modelName);

	void Render();
	void Update(FrameTime* _frameTime);

	ModelBase model;
};

