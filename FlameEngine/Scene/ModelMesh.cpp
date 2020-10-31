#include "ModelMesh.h"


ModelMesh::ModelMesh(int id)
{
	ID = id;
	World = identity<Matrix4>();
}
void ModelMesh::Render(Matrix4 Transform, Camera* _cam)
{

	_shader->UseProgram();
	_shader->SetMatrix("View", _cam->View);
	_shader->SetMatrix("Projection", _cam->Projection);
	_shader->SetMatrix("World", World * Transform);
	_shader->SetTexture(0, _material->colorMap);


	_vbo.RenderIndexed(GL_TRIANGLES);


	for (auto _modelMesh : children)
	{
		_modelMesh.Render(World * Transform, _cam);
	}
}