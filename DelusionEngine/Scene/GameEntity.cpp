#include "GameEntity.h"


GameEntity::GameEntity(STRING _modelName)
{

	AssetManager::LoadModel(_modelName, &model);

	cout << model.children.size() << endl;
	cout << &model<< endl;
}

void GameEntity::Render()
{
	model.Render();
}