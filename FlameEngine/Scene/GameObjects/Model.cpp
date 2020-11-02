#include "Model.h"

void Model::Render(Matrix4 _transform, Camera* _cam)
{

	_shader->UseProgram();
	_shader->SetMatrix("View", _cam->View);
	_shader->SetMatrix("Projection", _cam->Projection);
	_shader->SetMatrix("World", _transform * World);
	_shader->SetTexture(0, _material->colorMap);


	_vbo.RenderIndexed(GL_TRIANGLES);
}
