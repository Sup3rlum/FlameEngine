#include "GameEntity.h"
#include "../Scene.h"

GameEntity::GameEntity(STRING _modelName)
{

	AssetManager::LoadModel(_modelName, &model);

}

void GameEntity::Render()
{
	model.Render(identity<fMatrix4>(), _scene->_camera);
}