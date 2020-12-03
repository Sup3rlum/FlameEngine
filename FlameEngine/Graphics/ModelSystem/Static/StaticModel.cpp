#include "StaticModel.h"

void StaticModel::Render(Scene* scene)
{
	for (int i = 0; i < meshCollection.size(); i++)
	{
		meshCollection[i].Render(scene, World);
	}
}

void StaticModel::Update()
{
	for (int i = 0; i < meshCollection.size(); i++)
	{
		meshCollection[i].Update();
	}
}