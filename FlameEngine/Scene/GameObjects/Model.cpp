#include "Model.h"

void Model::Render(fMatrix4 _transform, Camera* _cam)
{

	_shader->UseProgram();
	_shader->SetMatrix("View", _cam->View);
	_shader->SetMatrix("Projection", _cam->Projection);
	_shader->SetMatrix("World", identity<fMatrix4>());
	_shader->SetTexture(0, _material->colorMap);


	_vbo.RenderIndexed(GL_TRIANGLES);
}
