#include "Sky.h"

#include "../Scene.h"

Sky::Sky(Scene* _sky)
{
	_scene = _sky;
}

void Sky::Render()
{

	glCullFace(GL_FRONT);

	_skyboxModel.Render(NULL, _scene);

	glCullFace(GL_BACK);

}

