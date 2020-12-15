#include "StaticModel.h"

void StaticModel::Render(Program* pRenderingProgram, Scene* scene)
{
	for (int i = 0; i < meshCollection.size(); i++)
	{
		meshCollection[i].Render(pRenderingProgram, scene, World);
	}
}

void StaticModel::Update()
{
	for (int i = 0; i < meshCollection.size(); i++)
	{
		meshCollection[i].Update();
	}
}