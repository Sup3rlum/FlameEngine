#include "GameEntity.h"
#include "../Scene.h"

GameEntity::GameEntity(STRING _modelName)
{

	AssetManager::LoadModel(_modelName, &model);

}

void GameEntity::Render()
{
	model.Render(fMatrix4::Translation(Position) * fMatrix4::Scaling( Scale) , _scene);
}


void GameEntity::Update(FrameTime* _frameTime)
{

}
