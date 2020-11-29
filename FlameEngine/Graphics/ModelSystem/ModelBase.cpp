#include "ModelBase.h"

#include "../../Scene/Scene.h"

void ModelBase::Render(fMatrix4 _transform, Scene* _scene)
{

	_shader->UseProgram();

	_shader->SetMatrix("View",			_scene->CurrentCamera()->View);
	_shader->SetMatrix("Projection",	_scene->CurrentCamera()->Projection);
	_shader->SetMatrix("World",			_transform);// _transform);

	_shader->SetTexture(0, _material->mColorMap);


	_vbo.RenderIndexed(GL_TRIANGLES);
}
