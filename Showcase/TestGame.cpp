#include "TestGame.h"

#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/Mesh.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/SkinnedMesh.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/Material.h"
#include "FlameEngine/Core/Engine/ContentSystem/Client/AssetImportScripts/AnimationSequence.h"

TestGameApplication::TestGameApplication(FString appName) : GameApplication(appName)
{

	playerEntity = currentScene->CreateEntity<TransformComponent, CameraComponent, FirstPersonCharacterComponent, ControlComponent>("player");
	currentScene->Camera = playerEntity;


	playerEntity.SetComponent(CameraComponent(FViewMatrix(FVector3(5, 5, 5), FVector3(0, 0, 0), FVector3(0, 1, 0)), FPerspectiveMatrix(PI / 4, 16.0f / 9.0f, 0.1f, 300.0f)));
	playerEntity.Component<TransformComponent>().Transform.Position = FVector3(5, 5, 5);
	playerEntity.Component<TransformComponent>().Transform.Orientation = FQuaternion::Identity();

	playerEntity.Component<FirstPersonCharacterComponent>().yaw = 2.3;
	playerEntity.Component<FirstPersonCharacterComponent>().flySpeed = 100.0;




	currentScene->Sun = currentScene->CreateEntity<DirectionalLightComponent>("Sun");
	currentScene->Sun.Component<DirectionalLightComponent>().Direction = FVector3::Normalize(FVector3(-1, -1, -1));
	currentScene->Sun.Component<DirectionalLightComponent>().Intensity = 1.0f;
	/*currentScene->Sun.Component<DirectionalLightComponent>().VMatrix = FViewMatrix(FVector3(30, 30, 0), FVector3(0), FVector3(0, 1, 0));
	currentScene->Sun.Component<DirectionalLightComponent>().PMatrix = FOrthographicMatrix(-30, 30, -30, 30, 0.1f, 50.0f);*/
}


void TestGameApplication::Update()
{
	FirstPersonCharacterSystem playerSystem = currentScene->CreateSystem<TransformComponent, CameraComponent, FirstPersonCharacterComponent, ControlComponent>();


	playerSystem.ForEach([&](Entity ent, TransformComponent& transformComponent, CameraComponent& cam, FirstPersonCharacterComponent& fp, ControlComponent& control)
		{

			if (control.IsKeyDown(FKeyboardKeys::Z))
			{
				fp.isCursorLocked = !fp.isCursorLocked;
			}
			
			if (control.IsKeyDown(FKeyboardKeys::A))
			{
				transformComponent.Transform.Position -= fp.Right * fp.flySpeed * 0.001f;
			}
			if (control.IsKeyDown(FKeyboardKeys::D))
			{
				transformComponent.Transform.Position += fp.Right * fp.flySpeed * 0.001f;
			}

			if (control.IsKeyDown(FKeyboardKeys::W))
			{
				transformComponent.Transform.Position += fp.LookDirection * fp.flySpeed * 0.001f;

			}

			if (control.IsKeyDown(FKeyboardKeys::S))
			{
				transformComponent.Transform.Position -= fp.LookDirection * fp.flySpeed * 0.001f;
			}

			if (control.IsKeyDown(FKeyboardKeys::K))
			{
				boneGuy.Component<AnimationComponent>().PlaySequence("Run");
			}



			if (control.IsKeyDown(FKeyboardKeys::Space))
			{
				isShooting = true;

			}
			if (!control.IsKeyDown(FKeyboardKeys::Space))
			{
				isShooting = false;
			}

			if (!wasShooting)
			{
				if (isShooting)
				{
					Shoot();
				}
			}


			fp.pitch = FMathFunc::Clamp
			(
				fp.pitch,
				-HALF_PI + 0.1f,
				HALF_PI - 0.1f
			);


			FVector2 cursorPos = FriContext->GetCursorPosition();

			if (fp.isCursorLocked)
			{

				FVector2 viewportSize = FriContext->GetViewportSize();

				FriContext->SetCursorPosition
				(
					viewportSize / 2.0f
				);


				FVector2 offset = (viewportSize / 2.0f - cursorPos) * 0.3f * -0.003f;

				fp.yaw -= offset.x;
				fp.pitch -= offset.y;
			}

			
			fp.LookDirection = FVector3
			(
				FMathFunc::Cos(fp.pitch) * FMathFunc::Sin(fp.yaw),
				FMathFunc::Sin(fp.pitch),
				FMathFunc::Cos(fp.pitch) * FMathFunc::Cos(fp.yaw)
			);
			
			fp.Right = FVector3
			(
				FMathFunc::Sin(fp.yaw - HALF_PI),
				0,
				FMathFunc::Cos(fp.yaw - HALF_PI)
			);
			
			fp.Up = fp.Right ^ fp.LookDirection;

			cam.View = FViewMatrix(transformComponent.Transform.Position, transformComponent.Transform.Position + fp.LookDirection, fp.Up);
		});

	wasShooting = isShooting;
}



void TestGameApplication::Shoot()
{
	
	FMeshLoader modelLoader(false, true);
	FMaterialLoader materialLoader(false, true);

	Entity ball = currentScene->CreateEntity<MeshComponent, MaterialComponent, TransformComponent, DynamicPhysicsComponent>("ball");

	ball.SetComponent<MeshComponent>(modelLoader.LoadFromLocal("models/sphere.fl3d", FriContext));
	ball.SetComponent<MaterialComponent>(materialLoader.LoadFromLocal("materials/default.flmt", FriContext));

	ball.SetComponent<TransformComponent>(FTransform(FVector3(0, 10, 0)));
	ball.SetComponent<DynamicPhysicsComponent>(currentScene->Physics->CreateDynamic(ball.Component<TransformComponent>().Transform));
	ball.Component<DynamicPhysicsComponent>().AttachShape(currentScene->Physics->CreateShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), SphereGeometry(1.0f)));
	ball.Component<DynamicPhysicsComponent>().SetAngularVelocity(FVector3(0.2f, 0, 0));
}

void TestGameApplication::Load()
{
	FMeshLoader modelLoader(false, true);
	FSkinnedMeshLoader skinnedModelLoader(false, true);
	FMaterialLoader materialLoader(false, true);
	FAnimSequenceLoader animLoader(false, true);


	floorEntity = currentScene->CreateEntity<MeshComponent, MaterialComponent, TransformComponent, StaticPhysicsComponent>("ground");

	floorEntity.SetComponent<MeshComponent>(modelLoader.LoadFromLocal("models/plane.fl3d", FriContext));
	floorEntity.SetComponent<MaterialComponent>(materialLoader.LoadFromLocal("materials/default.flmt", FriContext));

	floorEntity.Component<MaterialComponent>().SetWrapMode(EMaterialWrap::Repeat);
	floorEntity.Component<MaterialComponent>().SetFilterMode(EMaterialFilter::Trilinear);

	floorEntity.SetComponent<TransformComponent>(FTransform(FQuaternion::FromAxisAngle(-HALF_PI, FVector3(1, 0, 0))));
	floorEntity.SetComponent<StaticPhysicsComponent>(currentScene->Physics->CreateStatic(FTransform(FQuaternion::FromAxisAngle(HALF_PI, FVector3(0, 0, 1)))));

	floorEntity.Component<StaticPhysicsComponent>().AttachShape(currentScene->Physics->CreateShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), PlaneGeometry()));



	boneGuy = currentScene->CreateEntity<SkinnedMeshComponent, MaterialComponent, TransformComponent, AnimationComponent>("boneGuy");

	boneGuy.SetComponent<SkinnedMeshComponent>(skinnedModelLoader.LoadFromLocal("models/bone_guy.fl3d", FriContext));

	boneGuy.SetComponent<MaterialComponent>(materialLoader.LoadFromLocal("materials/default.flmt", FriContext));
	boneGuy.Component<MaterialComponent>().SetWrapMode(EMaterialWrap::Repeat);
	boneGuy.Component<MaterialComponent>().SetFilterMode(EMaterialFilter::Trilinear);

	boneGuy.SetComponent<TransformComponent>(FTransform(FQuaternion::FromAxisAngle(-HALF_PI, FVector3(1, 0, 0))));
	boneGuy.Component<AnimationComponent>() = AnimationComponent();

	boneGuy.Component<AnimationComponent>().AddSequence("Run", animLoader.LoadFromLocal("animations/anim_guy.fl3d.anim"));
}

void TestGameApplication::Suspend()
{


}

void TestGameApplication::Dispose()
{


}
/*/
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
}*/
/*
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
}*/