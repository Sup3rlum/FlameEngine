#include "TestGame.h"


#include <immintrin.h>
#include <ctime>

#include "FlameEngine/Mathematics/Types/FVector3.h"
#include "FlameEngine/Mathematics/Types/fMatrix4.h"

TestGame::TestGame(STRING name, DVERSION dv) : Game(name, dv)
{


}

void TestGame::Load()
{

	_currentScene->DirectionalLightCollection.push_back(DirectionalLight(FVector3(-1.0f, -1.0f, 0.2f), Color::White, 1.0f));

	//GameEntity* _man = new GameEntity("man.fl3d");
	GameEntity* _ground = new GameEntity("plane.fl3d");
	GameEntity* _box = new GameEntity("box.fl3d");
	//GameEntity* _sponza = new GameEntity("sponza.fl3d");
	GameEntity* _house = new GameEntity("cottage.fl3d");

	//_sponza->Position = FVector3(0, 0, 0);
	//_man->Position = FVector3(4, -1, -4);
	_box->Position = FVector3(0, 1, 0);
	_ground->Position = FVector3(0, 0, 0);
	//_house->Position = FVector3(35, 0.5f, -15);

	_ground->Scale = FVector3(5.0f);
	_box->Scale = FVector3(1.0f);
	//_sponza->Scale = FVector3(0.15f);
	_house->Scale = FVector3(1.0f);
	//_man->Scale = FVector3(1.0f);


	PxMaterial* pxMaterial = _currentScene->pPhysXService->mPxPhysics->createMaterial(0.5, 0.5, 0.1);

	_box->pPxActor = _currentScene->pPhysXService->mPxPhysics->createRigidDynamic(PxTransform(PxVec3(0, 50, 0), PxQuat(0.3, PxVec3(1,1,0).getNormalized())));
	PxShape* boxShape = _currentScene->pPhysXService->mPxPhysics->createShape(PxBoxGeometry(1.0, 1.0, 1.0), *pxMaterial);
	_box->pPxActor->attachShape(*boxShape);


	_ground->pPxActor = _currentScene->pPhysXService->mPxPhysics->createRigidStatic(PxTransform(PxVec3(0, 0, 0),PxQuat(HALF_PI, PxVec3(0,0,1))));
	PxShape* _groundShape = _currentScene->pPhysXService->mPxPhysics->createShape(PxPlaneGeometry(), *pxMaterial);
	_ground->pPxActor->attachShape(*_groundShape);



	GameEntity* _box2 = new GameEntity("box.fl3d");

	_box2->Position = FVector3(4, -1, -4);
	_box2->Scale = FVector3(1.0f);

	_box2->pPxActor = _currentScene->pPhysXService->mPxPhysics->createRigidDynamic(PxTransform(PxVec3(0, 50, 0), PxQuat(0.3, PxVec3(1, 1, 0).getNormalized())));
	_box2->pPxActor->attachShape(*boxShape);



	//_currentScene->AddActor("man", _man);
	_currentScene->AddActor("box", _box);
	_currentScene->AddActor("rbox", _box2);
	_currentScene->AddActor("ground", _ground);
	//_currentScene->AddActor("sponza", _sponza);
	_currentScene->AddActor("house", _house);


	


	Game::Load();
}

void TestGame::Suspend()
{


}

void TestGame::Dispose()
{


}


void TestGame::KeyEventCallback(KeyEventArgs args)
{
	if (args.Key == Keys::LEFT_ALT && args.keyState == KeyState::PRESSED)
	{
		((FpsCamera*)_currentScene->CurrentCamera())->SetCursorLocked(!((FpsCamera*)_currentScene->CurrentCamera())->cursordLocked);
	}

	if (args.Key == Keys::SPACE && args.keyState == KeyState::PRESSED)
	{


		GameEntity* _box = new GameEntity("box.fl3d");

		_box->Scale = FVector3(1.0f);

		PxMaterial* pxMaterial = _currentScene->pPhysXService->mPxPhysics->createMaterial(0.5, 0.5, 0.1);

		_box->pPxActor = _currentScene->pPhysXService->mPxPhysics->createRigidDynamic(PxTransform(PxVec3(0, 50, 0), PxQuat(0.3, PxVec3(1, 1, 0).getNormalized())));
		PxShape* boxShape = _currentScene->pPhysXService->mPxPhysics->createShape(PxBoxGeometry(1.0, 1.0, 1.0), *pxMaterial);
		_box->pPxActor->attachShape(*boxShape);



		char buff[128];

		sprintf_s(buff, "box%d", _currentScene->actorCollection.size());

		_currentScene->AddActor(buff, _box);

		FLAME_INFO("Adding actor \'" + STRING(buff) + "\'");

	}

	if (args.Key == Keys::LEFT_SHIFT && args.keyState == KeyState::PRESSED)
	{


		FVector3 dir = _currentScene->CurrentCamera()->LookDirection;
		FVector3 pos = _currentScene->CurrentCamera()->Position;



		GameEntity* _box = new GameEntity("box.fl3d");

		_box->Scale = FVector3(1.0f);

		PxMaterial* pxMaterial = _currentScene->pPhysXService->mPxPhysics->createMaterial(0.5, 0.5, 0.1);

		_box->pPxActor = _currentScene->pPhysXService->mPxPhysics->createRigidDynamic(PxTransform(PxVec3(pos.x, pos.y, pos.z)));
		PxShape* boxShape = _currentScene->pPhysXService->mPxPhysics->createShape(PxBoxGeometry(1.0, 1.0, 1.0), *pxMaterial);
		_box->pPxActor->attachShape(*boxShape);





		_box->pPxActor->is<PxRigidDynamic>()->setLinearVelocity(PxVec3(dir.x, dir.y, dir.z) * 100);


		char buff[128];

		sprintf_s(buff, "box%d", _currentScene->actorCollection.size());

		_currentScene->AddActor(buff, _box);

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
}