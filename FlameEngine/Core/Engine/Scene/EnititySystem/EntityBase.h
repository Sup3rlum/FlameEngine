#pragma once

#include "Actor.h"
#include "../../FileSystem/AssetManager.h"



EXPORT(class,  GameEntity) : public Actor
{

public:
	GameEntity(STRING _modelName);

	void Render(Program* pRenderingProgram);
	void Update(FrameTime* _frameTime);

	StaticModel model;
};

