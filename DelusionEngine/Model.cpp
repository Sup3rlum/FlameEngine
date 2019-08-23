#include "Model.h"

void Model::Render()
{
	if (children != NULL)
	{

		for (auto _modelMesh : *children)
		{
			_modelMesh.Render();
		}
	}
}
