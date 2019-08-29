#include "Scene.h"



Scene::Scene(Context* _cont)
{

	_camera = new Camera(_cont, CameraType::FIRSTPERSON);

	DebugView::Init(_cont);
}

void Scene::Update()
{
	_camera->Update();

	DebugView::Update();
}
void Scene::Render()
{
	glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DebugView::Draw(_camera);

}