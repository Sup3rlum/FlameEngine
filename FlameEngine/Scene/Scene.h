#pragma once


#include "GameObjects/FpsCamera.h"
#include "GameObjects/Actor.h"
#include "GameObjects/Lighting/DirectionalLight.h"
#include "Environment/Sky.h"
#include "../Graphics/ShaderDefinitions/Shader.h"
#include "../Graphics/Common/RenderBatch.h"
#include "../Graphics/Common/FrameBuffer.h"
#include "../Context/Context.h"
#include "../FileSystem/AssetManager.h"
#include "../Mathematics/Module.h"
#include "Physics/PhysXService.h"

#include "../Graphics/UserInterface/UxService.h"
#include "../Graphics/UserInterface/UxFrame.h"
#include "../Graphics/UserInterface/UxLabel.h"
#include "../Graphics/UserInterface/UxConsoleWindow.h"
#include "../Graphics/UserInterface/UxDebugViewComponent.h"


EXPORT(class,  Scene)
{
public:
	Scene(Context* _context);
	~Scene();

	void Update();
	void Render();


	stack<Camera*> _cameraStack;

	void AddActor(STRING id, Actor* ac);
	void RemoveActor(STRING id);

	void PushCamera(Camera* _cam);
	Camera* PopCamera();
	Camera* CurrentCamera();


	std::vector<DirectionalLight> LightCollection;

	Context* _context;

	RenderBatch* _renderBatch;


	PhysXService* pPhysXService;
	PxScene* pPxScene;


	PxActor* _actors;


	UxService* pUxService;
	UxContainer* pUxContainer;

	UxLabel* pUxLabel;

	UxConsoleWindow* cons;

	std::unordered_map<STRING, Actor*> actorCollection;
};

