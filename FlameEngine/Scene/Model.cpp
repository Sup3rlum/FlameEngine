#include "Model.h"

void Model::Render(Matrix4 _transform, Camera* _cam)
{

	for (auto _modelMesh : children)
	{
		_modelMesh.Render( _transform, _cam);
	}
}
