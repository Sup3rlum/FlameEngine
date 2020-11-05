#include "GameEntity.h"
#include "../Scene.h"

GameEntity::GameEntity(STRING _modelName)
{

	AssetManager::LoadModel(_modelName, &model);

}

void GameEntity::Render()
{
	model.Render(translate(identity<fMatrix4>(), Position), _scene);
}

void GameEntity::RenderDepth()
{
	model.RenderDepth(translate(identity<fMatrix4>(), Position), _scene);
}