#include "ModelMesh.h"

void ModelMesh::Render()
{

	_vbo->RenderIndexed(GL_TRIANGLES);



	for (auto _modelMesh : children)
	{
		_modelMesh.Render();
	}
}