#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Framework/Common/FString.h"
#include "Core/Framework/Common/FStack.h"
#include "../CameraSystem/Camera.h"
#include "Core/Framework/Globals/FGlobalID.h"
#include "../Common/EntityBase.h"
#include "../Common/EntityGroup.h"
#include "Core/Framework/Common/FHashMap.h"



EXPORT(class,  Scene)
{
public:
	Scene();
	~Scene();

	void Update();
	void Render();


	FStack<Camera*> _cameraStack;

	void AddEntity(FString name, FSceneEntityBase* ac);
	void RemoveEntity(FGlobalID id);

	void PushCamera(Camera* _cam);
	void PopCamera();
	Camera* CurrentCamera();
	

	FHashMap<FGlobalID, FSceneEntityBase*> actorCollection;
};

