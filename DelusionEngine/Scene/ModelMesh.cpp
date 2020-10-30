#include "ModelMesh.h"


ModelMesh::ModelMesh(int id)
{
	ID = id;

}
void ModelMesh::Render()
{

	_vbo.RenderIndexed(GL_TRIANGLES);
	//_vbo.Render(GL_LINE_STRIP);
	//_vbo.Render(GL_TRIANGLES);

	//cout << glGetError() << endl;


	for (auto _modelMesh : children)
	{
		_modelMesh.Render();
	}
}