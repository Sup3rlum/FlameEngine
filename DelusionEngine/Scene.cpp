#include "Scene.h"



Scene::Scene()
{

	_camera = new Camera();

	DebugView::Init();
}

void Scene::Update( FrameTime* _frTime)
{
	_camera->Update( _frTime);

	DebugView::Update(_frTime);
}
void Scene::Render(FrameTime* _frTime)
{
	glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DebugView::Draw(_camera);

}