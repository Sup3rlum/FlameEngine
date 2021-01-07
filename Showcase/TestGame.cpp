#include "TestGame.h"


#include <immintrin.h>
#include <ctime>


TestGameApplication::TestGameApplication(FString appName) : GameApplication(appName)
{

}

void TestGameApplication::Load()
{

	_currentScene->DirectionalLightCollection.push_back(DirectionalLight(FVector3(-1.0f, -1.0f, 0.2f), Color::White, 1.0f));

	_currentScene->PointLightCollection.push_back(PointLight(FVector3(-20.0f, 1.0f, 0.0f), Color::Red,		1.0f, 10.0f));
	_currentScene->PointLightCollection.push_back(PointLight(FVector3(-10.0f, 5.0f, 0.0f), Color::Blue,		2.0f, 15.0f));
	_currentScene->PointLightCollection.push_back(PointLight(FVector3(0.0f,	 3.0f, 0.0f), Color::Green,		3.3f, 3.0f));
	_currentScene->PointLightCollection.push_back(PointLight(FVector3(10.0f, 5.0f, 0.0f), Color::Yellow,	0.5f, 25.0f));
	_currentScene->PointLightCollection.push_back(PointLight(FVector3(20.0f, 1.0f, 0.0f), Color::White,	7.0f, 1.3f));

	//GameEntity* _man = new GameEntity("man.fl3d");
	//_man->Position = FVector3(4, -1, -4);
	//_man->Scale = FVector3(1.0f);

	PxMaterial* pxMaterial = _currentScene->pPhysXService->mPxPhysics->createMaterial(0.8, 0.8, 0.1);

	GameEntity* _ground = new GameEntity("plane.fl3d");
	_ground->Position = FVector3(0, 0, 0);
	_ground->Scale = FVector3(5.0f);
	_ground->pPxActor = _currentScene->pPhysXService->mPxPhysics->createRigidStatic(PxTransform(PxVec3(0, 0, 0), PxQuat(HALF_PI, PxVec3(0, 0, 1))));
	PxShape* _groundShape = _currentScene->pPhysXService->mPxPhysics->createShape(PxPlaneGeometry(), *pxMaterial);
	_ground->pPxActor->attachShape(*_groundShape);
	_currentScene->AddActor("ground", _ground);



	/*GameEntity* _sponza = new GameEntity("sponza.fl3d");
	_sponza->Position = FVector3(0, 0, 0);
	_sponza->Scale = FVector3(0.15f);
	_currentScene->AddActor("sponza", _sponza);
	*/


	//GameEntity* _house = new GameEntity("cottage.fl3d");
	//_house->Scale = FVector3(1.0f);
	//_house->Position = FVector3(35, 0.5f, -15);






	//_currentScene->AddActor("man", _man);



	//_currentScene->AddActor("house", _house);


	


}

void TestGameApplication::Suspend()
{


}

void TestGameApplication::Dispose()
{


}

void TestGameApplication::ShootSphere()
{
	FVector3 dir = _currentScene->CurrentCamera()->LookDirection;
	FVector3 pos = _currentScene->CurrentCamera()->Position;



	GameEntity* _sphere = new GameEntity("sphere.fl3d");

	_sphere->Scale = FVector3(1.0f);

	PxMaterial* pxMaterial = _currentScene->pPhysXService->mPxPhysics->createMaterial(0.5, 0.5, 0.1);

	_sphere->pPxActor = _currentScene->pPhysXService->mPxPhysics->createRigidDynamic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));
	PxShape* sphereShape = _currentScene->pPhysXService->mPxPhysics->createShape(PxSphereGeometry(1.0), *pxMaterial);
	_sphere->pPxActor->attachShape(*sphereShape);


	_sphere->pPxActor->is<PxRigidDynamic>()->setMass(10.0f);
	_sphere->pPxActor->is<PxRigidDynamic>()->setAngularDamping(0.5f);
	_sphere->pPxActor->is<PxRigidDynamic>()->setLinearVelocity(PxVec3(dir.x, dir.y, dir.z) * 10);


	char buff[128];

	sprintf_s(buff, "sphere%d", _currentScene->actorCollection.size());

	_currentScene->AddActor(buff, _sphere);
}
void TestGameApplication::ShootBox()
{
	FVector3 dir = _currentScene->CurrentCamera()->LookDirection;
	FVector3 pos = _currentScene->CurrentCamera()->Position;



	GameEntity* _sphere = new GameEntity("box.fl3d");

	_sphere->Scale = FVector3(1.0f);

	PxMaterial* pxMaterial = _currentScene->pPhysXService->mPxPhysics->createMaterial(0.5, 0.5, 0.1);

	_sphere->pPxActor = _currentScene->pPhysXService->mPxPhysics->createRigidDynamic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));
	PxShape* sphereShape = _currentScene->pPhysXService->mPxPhysics->createShape(PxBoxGeometry(1.0, 1.0, 1.0), *pxMaterial);
	_sphere->pPxActor->attachShape(*sphereShape);


	_sphere->pPxActor->is<PxRigidDynamic>()->setMass(10.0f);
	_sphere->pPxActor->is<PxRigidDynamic>()->setAngularDamping(0.5f);
	_sphere->pPxActor->is<PxRigidDynamic>()->setLinearVelocity(PxVec3(dir.x, dir.y, dir.z) * 100);


	char buff[128];

	sprintf_s(buff, "box%d", _currentScene->actorCollection.size());

	_currentScene->AddActor(buff, _sphere);
}


void TestGameApplication::KeyEventCallback(KeyEventArgs args)
{
	if (args.Key == Keys::LEFT_ALT && args.keyState == KeyState::PRESSED)
	{
		((FpsCamera*)_currentScene->CurrentCamera())->SetCursorLocked(!((FpsCamera*)_currentScene->CurrentCamera())->cursordLocked);
	}

	if (args.Key == Keys::SPACE && args.keyState == KeyState::PRESSED)
	{
		ShootSphere();
	}

	if (args.Key == Keys::LEFT_SHIFT && args.keyState == KeyState::PRESSED)
	{


		ShootBox();

	}

	if (args.Key == Keys::GRAVE_ACCENT && args.keyState == KeyState::PRESSED)
	{
		_currentScene->cons->ToggleConsole();
	}
	if (args.Key == Keys::J && args.keyState == KeyState::PRESSED)
	{
		EngineInstance::_handlingInstance->mRenderer->mBlurPassIndex = (EngineInstance::_handlingInstance->mRenderer->mBlurPassIndex + 1) % 4;
	}

	if (args.Key == Keys::H && args.keyState == KeyState::PRESSED)
	{
		EngineInstance::_handlingInstance->mRenderer->enableHBAO = !EngineInstance::_handlingInstance->mRenderer->enableHBAO;
	}

	if (args.Key == Keys::K && args.keyState == KeyState::PRESSED)
	{
		EngineInstance::_handlingInstance->mRenderer->enableGlossMap = !EngineInstance::_handlingInstance->mRenderer->enableGlossMap;
	}

	if (args.Key == Keys::TAB && args.keyState == KeyState::PRESSED)
	{
		EngineInstance::_handlingInstance->mRenderer->enableDEBUGTexture = !EngineInstance::_handlingInstance->mRenderer->enableDEBUGTexture;
	}

	if (args.Key == Keys::P && args.keyState == KeyState::RELEASED)
	{
		_currentScene->PopCamera();
	}	


	if (args.Key == Keys::LEFT_CONTROL && args.keyState == KeyState::PRESSED)
	{
		((FpsCamera*)_currentScene->CurrentCamera())->flySpeed = 40;
	}	
	if (args.Key == Keys::LEFT_CONTROL && args.keyState == KeyState::RELEASED)
	{
		((FpsCamera*)_currentScene->CurrentCamera())->flySpeed = 5;
	}

	if (args.Key == Keys::ENTER && args.keyState == KeyState::RELEASED)
	{
		EngineInstance::_handlingInstance->mRenderer->ScreenShot();
	}

	if (args.Key == Keys::N && args.keyState == KeyState::RELEASED)
	{
		_currentScene->snapToFrust = !_currentScene->snapToFrust;
	}
}