#pragma once


#include "GameObjects/FpsCamera.h"
#include "GameObjects/Actor.h"
#include "GameObjects/Lighting/DirectionalLight.h"
#include "GameObjects/Lighting/AmbientLight.h"
#include "GameObjects/Lighting/SpotLight.h"
#include "GameObjects/Lighting/PointLight.h"
#include "Environment/Sky.h"
#include "../Graphics/ShaderDefinitions/Shader.h"
#include "../Graphics/Common/RenderBatch.h"
#include "../Graphics/Common/FrameBuffer.h"
#include "../Context/Context.h"
#include "../FileSystem/AssetManager.h"
#include "../Mathematics/Module.h"
#include "Physics/PhysXService.h"
#include "GameObjects/Visual/LensFlare.h"

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

	int GetLightsCount();



	std::vector<DirectionalLight> DirectionalLightCollection;
	std::vector<AmbientLight> AmbientLightCollection;
	std::vector<SpotLight> SpotLightCollection;
	std::vector<PointLight> PointLightCollection;

	Context* _context;


	PhysXService* pPhysXService;
	PxScene* pPxScene;


	PxActor* _actors;


	UxService* pUxService;
	UxContainer* pUxContainer;

	UxConsoleWindow* cons;


	float lightingIndex;

	std::unordered_map<STRING, Actor*> actorCollection;
};

