#include "Model.h"

#include "../Scene.h"
/*
void Model::Render(fMatrix4 _transform, Scene* _scene)
{

	_shader->UseProgram();

	_shader->SetMatrix("View",			_scene->_camera->View);
	_shader->SetMatrix("Projection",	_scene->_camera->Projection);


	_shader->SetMatrix("LightViewProjection", _scene->LightCollection[0].Projection * _scene->LightCollection[0].View);


	_shader->SetMatrix("World",			_transform);


	int diffLocation = glGetUniformLocation(_shader->_programID, "_texture");
	int shadowLocation = glGetUniformLocation(_shader->_programID, "_shadowMap");

	glUniform1i(diffLocation, 0);
	glUniform1i(shadowLocation, 1);

	_shader->SetTexture(0,				_material->colorMap);
	_shader->SetTexture(1,				_scene->_depthMap);

	for (int i = 0; i < _scene->LightCollection.size(); i++)
	{

		_shader->SetVector("DirectionalLights[0].Direction", _scene->LightCollection[0].Direction);
		_shader->SetVector("DirectionalLights[0].Color", _scene->LightCollection[0].LightColor);
		_shader->SetFloat("DirectionalLights[0].Intensity", _scene->LightCollection[0].Intensity);
	}

	_vbo.RenderIndexed(GL_TRIANGLES);
}
void Model::RenderDepth(fMatrix4 _transform, Scene* _scene)
{

	_depthShader->UseProgram();


	/*_depthShader->SetMatrix("View",			_scene->_camera->View);
	_depthShader->SetMatrix("Projection",	_scene->_camera->Projection);


	_depthShader->SetMatrix("View",			_scene->LightCollection[0].View);
	_depthShader->SetMatrix("Projection",	_scene->LightCollection[0].Projection);


	_depthShader->SetMatrix("World", _transform);


	_vbo.RenderIndexed(GL_TRIANGLES);
}*/
void Model::Render(fMatrix4 _transform, Scene* _scene)
{

	_shader->UseProgram();

	_shader->SetMatrix("View",			_scene->CurrentCamera()->View);
	_shader->SetMatrix("Projection",	_scene->CurrentCamera()->Projection);
	_shader->SetMatrix("World",			_transform);

	_shader->SetTexture(0, _material->colorMap);


	_vbo.RenderIndexed(GL_TRIANGLES);
}
