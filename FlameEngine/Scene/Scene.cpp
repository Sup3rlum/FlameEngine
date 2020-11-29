#include "Scene.h"


GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };




Scene::Scene(Context* _cont)
{
	_context = _cont;


	pPhysXService = new PhysXService();


	pUxService = new UxService();

	pUxContainer = new UxContainer(_cont);
	pUxService->PushContainer(pUxContainer);

	cons = new UxConsoleWindow();
	cons->Position = fVector2(1600, 300);
	cons->Size = fVector2(500, 500);

	pUxContainer->AddComponent(new UxDebugViewComponent(this));
	pUxContainer->AddComponent(cons);


	if (pPhysXService->InitializePhysX() == FLRESULT::FAIL)
	{
		printf("Something is wrong!\n");
	}

	pPxScene = pPhysXService->CreateScene();


	if (pPxScene == NULL)
	{
		printf("Scene is wrong!\n");
	}
	else
	{
		FLAME_INFO("PhysX Scene initialized");
	}

	PushCamera(new FpsCamera(_cont));


	_renderBatch = new RenderBatch(_cont);

	FLAME_INFO("Scene finished loading");
}

void Scene::Update()
{

	_cameraStack.top()->Update();

	for (auto i = actorCollection.begin(); i != actorCollection.end(); i++)
	{
		i->second->Update(NULL);
	}


	pPxScene->simulate(1.0f / 60.0f);
	pPxScene->fetchResults(true);


	pUxService->Update();

}

void Scene::Render()
{
	for (auto i = actorCollection.begin(); i != actorCollection.end(); i++)
	{
		i->second->Render();
	}

}

void Scene::AddActor(std::string _id, Actor* _ac)
{
	if (actorCollection.find(_id) == actorCollection.end())
	{

		_ac->_scene = this;

		actorCollection[_id] = _ac;

		if (_ac->pPxActor != NULL)
		{
			pPxScene->addActor(*_ac->pPxActor);

			//printf("%s has pxActor with adress: %p\n", _id.c_str(), _ac->pPxActor);

		}
	}

}
void Scene::RemoveActor(std::string _id)
{
	actorCollection.erase(_id);

}

void Scene::PushCamera(Camera* _cam)
{
	if (_cam != NULL)
	{
		_cameraStack.push(_cam);
	}
}


Camera* Scene::PopCamera()
{
	if (!_cameraStack.size())
		return NULL;

	Camera* _cameraPtr = _cameraStack.top();

	_cameraStack.pop();

	if (CurrentCamera() == NULL)
	{
		PopCamera();
	}

	return _cameraPtr;
}

inline Camera* Scene::CurrentCamera()
{
	return _cameraStack.top();
}

