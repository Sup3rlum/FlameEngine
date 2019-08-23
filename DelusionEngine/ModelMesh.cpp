#include "ModelMesh.h"

void ModelMesh::Render()
{

	_vbo->RenderIndexed(GL_TRIANGLES);

	if (children != NULL)
	{

		for (auto _modelMesh : *children)
		{
			_modelMesh.Render();
		}
	}
}