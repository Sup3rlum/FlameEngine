#include "Model.h"

void Model::Render()
{

	for (auto _modelMesh : children)
	{
		_modelMesh.Render();
	}
}
