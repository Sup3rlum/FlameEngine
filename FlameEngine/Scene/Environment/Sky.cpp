#include "Sky.h"

#include "../Scene.h"

Sky::Sky(Scene* _sky)
{
	_scene = _sky;
}

void Sky::Render()
{

	glCullFace(GL_FRONT);

	_skyboxModel.Render(translate(identity<fMatrix4>(), _scene->CurrentCamera()->Position), _scene);

	glCullFace(GL_BACK);

}

