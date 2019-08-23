#include "Scene.h"



Scene::Scene(ContextParameters* params)
{
	_params = params;


	_camera = new Camera();


	DebugView::Init();

}

void Scene::Update(GLFWwindow* _win, FrameTime* _frTime)
{
	_camera->Update(_win, _params, _frTime);
}
void Scene::Render(FrameTime* _frTime)
{


	DebugView::Draw(_camera);
}