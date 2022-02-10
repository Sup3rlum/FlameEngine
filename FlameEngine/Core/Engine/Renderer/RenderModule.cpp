#include "RenderModule.h"


void RenderModule::AttachToScene(Scene* scene)
{

	if (scene)
	{
		this->scene = scene;

		for (int i = 0; i < RenderModules.Length(); i++)
		{
			RenderModules[i]->AttachToScene(scene);
		}
	}
}
