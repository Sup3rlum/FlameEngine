#include "NativeEditorApp.h"


#include "Platform/Windows/Win32Context.h"

#include "Core/Engine/ContentSystem/Client/AssetImportScripts/Mesh.h"
#include "Core/Engine/ContentSystem/Client/AssetImportScripts/SkinnedMesh.h"
#include "Core/Engine/ContentSystem/Client/AssetImportScripts/Material.h"
#include "Core/Engine/ContentSystem/Client/AssetImportScripts/AnimationSequence.h"

namespace FlameEditorCLR
{

	EditorApplication::EditorApplication(Win32Context* editorContext) : GameApplication("Flame Editor (64-bit)", EFRIRendererFramework::OpenGL, editorContext)
	{
		currentScene->Camera = currentScene->CreateEntity<TransformComponent, CameraComponent, FirstPersonComponent, ControlComponent>("player");


		float aspectRatio = FriContext->GetViewport().AspectRatio();

		currentScene->Camera.SetComponent(CameraComponent(FViewMatrix(FVector3(5, 5, 5), FVector3(0, 0, 0), FVector3(0, 1, 0)), FPerspectiveMatrix(PI / 4, aspectRatio, 0.1f, 300.0f)));
		currentScene->Camera.Component<TransformComponent>().Transform.Position = FVector3(5, 5, 5);
		currentScene->Camera.Component<TransformComponent>().Transform.Orientation = FQuaternion::Identity();

		currentScene->Camera.Component<FirstPersonComponent>().yaw = 2.3;
		currentScene->Camera.Component<FirstPersonComponent>().flySpeed = 100.0;


		currentScene->Sun = currentScene->CreateEntity<DirectionalLightComponent>("Sun");
		currentScene->Sun.Component<DirectionalLightComponent>().Direction = FVector3::Normalize(FVector3(-1, -1, -1));
		currentScene->Sun.Component<DirectionalLightComponent>().Intensity = 1.0f;
	}


	void EditorApplication::Update()
	{

		auto playerSystem = currentScene->CreateSystem<TransformComponent, CameraComponent, FirstPersonComponent, ControlComponent>();


		playerSystem.ForEach([&](Entity ent, TransformComponent& transformComponent, CameraComponent& cam, FirstPersonComponent& fp, ControlComponent& control)
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



				fp.pitch = FMathFunc::Clamp
				(
					fp.pitch,
					-HALF_PI + 0.1f,
					HALF_PI - 0.1f
				);


				FVector2 cursorPos = FriContext->GetCursorPosition();

				if (control.IsMouseButtonDown(FMouseButton::Right))
				{
					FVector2 viewportSize = FriContext->GetViewport().Size;

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
	}

	void EditorApplication::Load()
	{
		FMeshLoader modelLoader(false, true);
		FSkinnedMeshLoader skinnedModelLoader(false, true);
		FMaterialLoader materialLoader(false, true);
		FAnimSequenceLoader animLoader(false, true);


		floorEntity = currentScene->CreateEntity<MeshComponent, MaterialComponent, TransformComponent, StaticPhysicsComponent>("ground");

		floorEntity.SetComponent<MeshComponent>(modelLoader.LoadFromLocal("models/plane2.fl3d", FriContext));
		floorEntity.SetComponent<MaterialComponent>(materialLoader.LoadFromLocal("materials/default.flmt", FriContext));

		floorEntity.Component<MaterialComponent>().SetWrapMode(EMaterialWrap::Repeat);
		floorEntity.Component<MaterialComponent>().SetFilterMode(EMaterialFilter::Trilinear);

		floorEntity.SetComponent<TransformComponent>(FTransform());
		floorEntity.SetComponent<StaticPhysicsComponent>(currentScene->Physics->CreateStatic(FTransform(FQuaternion::FromAxisAngle(HALF_PI, FVector3(0, 0, 1)))));

		floorEntity.Component<StaticPhysicsComponent>().AttachShape(currentScene->Physics->CreateShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), PlaneGeometry()));



		boneGuy = currentScene->CreateEntity<SkinnedMeshComponent, MaterialComponent, TransformComponent, AnimationComponent>("boneGuy");

		boneGuy.SetComponent<SkinnedMeshComponent>(skinnedModelLoader.LoadFromLocal("models/bone_guy.fl3d", FriContext));

		boneGuy.SetComponent<MaterialComponent>(materialLoader.LoadFromLocal("materials/default.flmt", FriContext));
		boneGuy.Component<MaterialComponent>().SetWrapMode(EMaterialWrap::Repeat);
		boneGuy.Component<MaterialComponent>().SetFilterMode(EMaterialFilter::Trilinear);

		boneGuy.SetComponent<TransformComponent>(FTransform());
		boneGuy.SetComponent<AnimationComponent>();

		boneGuy.Component<AnimationComponent>().AddSequence("Run", animLoader.LoadFromLocal("animations/anim_guy.fl3d.anim"));
	}
}