#include "Scene.h"
#include "Core/Math/Geometry/Volumes/AABB.h"


#include "../Physics/PX/FPXService.h"
#include "../Physics/PX/FPXScene.h"
#include "../Physics/PX/FPXAllocator.h"

float cascadeBias = 35.0f;




void SplitFrustum(FStaticArray<FVector3, 8>& source, FStaticArray<FVector3, 8>& out, float maxSplits, float splitIndex)
{

	float len0 = (splitIndex / maxSplits);
	float len1 = ((splitIndex + 1) / maxSplits);

	for (int i = 0; i < 4; i++)
	{
		out[i] = (source[i + 4] - source[i]) * len0 + source[i];
	}

	for (int i = 4; i < 8; i++)
	{
		out[i] = (source[i] - source[i-4]) * len1 + source[i-4];
	}
}


Scene::Scene(FString Name, class GameApplication* Game, FRIContext* renderContext, PhysicsDescription desc) :
	FriContext(renderContext),
	Game(Game)
{

	uxContainer = new UXContainer(FriContext);

	FPXService* fpxService = new FPXService();
	PhysicsScene* fpxScene = fpxService->CreateScene(FVector3(0, -10.0f, 0));

	physicsService = fpxService;
	physicsScene = fpxScene;
	Physics = new FPXAllocator(fpxService, static_cast<FPXScene*>(fpxScene));

}


void Scene::UpdateSystems()
{
	this->System<RigidBody, FTransform>()->ParallelForEach([&](Entity entity, RigidBody& body, FTransform& transform)
		{
			transform = body.GetGlobalTransform();
		});
}

void Scene::UpdateBehaviour(FGameTime gameTime)
{
	this->System<Behaviour>()->ParallelForEach([&](Entity entity, Behaviour& behaviour)
		{
			if (behaviour.pScript)
			{
				behaviour.pScript->Update(gameTime.DeltaTime.GetSeconds());
			}
		});

	this->System<AnimationComponent>()->ParallelForEach([&](Entity ent, AnimationComponent& animation)
		{
			animation.Step(gameTime.DeltaTime.GetSeconds());
		});

	this->System<RiggedModel, AnimationComponent>()->ParallelForEach([&](Entity ent, RiggedModel& skinnedModel, AnimationComponent& animation)
		{
			skinnedModel.Mesh.MeshSkeleton.ApplyPose(animation.CurrentAnimationPose);
		});

}

void Scene::Update(FGameTime gameTime)
{

	uxContainer->UpdateContainer();
	physicsScene->Step(gameTime.DeltaTime.GetSeconds());

	UpdateBehaviour(gameTime);

	UpdateSystems();
	for (auto sysPtr : Systems)
	{
		sysPtr->Tick();
	}

	System<ParticleManager>()->ParallelForEach([&](Entity ent, ParticleManager& particleManager)
		{
			particleManager.ParticleSystemPtr->Tick(gameTime.DeltaTime.GetSeconds(), Elements["GameCamera"].Component<Camera>());
		});

	UpdateDirectionalLights();
}

void Scene::UpdateDirectionalLights()
{
	if (!Elements.Contains("GameCamera") || !Elements["GameCamera"].HasComponent<Camera>())
		return;

	Camera& camera = Elements["GameCamera"].Component<Camera>();

	FStaticArray<FVector3, 8> frustumCorners;
	camera.GetFrustumCorners(frustumCorners);

	System<DirectionalLight>()->ParallelForEach([&](Entity ent, DirectionalLight& dirLight)
		{
			FStaticArray<FVector3, 8> frustumSplitCorners;
			{
				// Create an orthonormal basis describing the light's local coordinate system

				FViewMatrix viewMatrix(FVector3(0), dirLight.Direction, FVector3(0, 1, 0));

				FMatrix3 toGlobalSpace = FMatrix4::ToMatrix3(viewMatrix);
				FMatrix3 toLocalSpace = FMatrix3::Transpose(toGlobalSpace);

				for (int i = 0; i < SM_CASCADES; i++)
				{
					SplitFrustum(frustumCorners, frustumSplitCorners, SM_CASCADES, i);

					// Create the AABB enveloping the users view frustum in the light's basis space
					AABB aabb;
					aabb.SetDegenerate();

					for (int i = 0; i < 8; i++)
					{
						FVector3 p = toLocalSpace * frustumSplitCorners[i];
						aabb.Enclose(p);
					}

					// Get the position of the camera as being in the middle of the -Z plane of the AABB, add bias, and then turn it into global space
					FVector3 position = toGlobalSpace * (aabb.Center() - FVector3(0, 0, aabb.LengthZ() / 2.0f + cascadeBias));

					float halfLengthX = aabb.LengthX() / 2.0f;
					float halfLengthY = aabb.LengthY() / 2.0f;

					//Get the up component required to reorient the AABB into global space
					FVector3 aabbUp = toGlobalSpace * FVector3(0, 1, 0);

					// Create the view and projection matrices for the light's camera that envelops the user view frustum
					dirLight.FrustumData[i].View = FViewMatrix(position, position + dirLight.Direction, aabbUp);
					dirLight.FrustumData[i].Projection = FOrthographicMatrix(-halfLengthX, halfLengthX, -halfLengthY, halfLengthY, 0.0f, aabb.LengthZ() + cascadeBias);

					float zFar = camera.FarPlane();
					float zNear = camera.NearPlane();

					dirLight.FrustumData[i].Depth = (zFar - zNear) * ((float)(i + 1) / (float)SM_CASCADES) + zNear;
				}
			}
		});
}


AABB Scene::GetAABB() const
{

	return AABB(0, 0);
}

FArray<Entity> Scene::QueryEntities(const FString& name)
{
	FArray<Entity> result;

	if (name.Length() == 0)
		return result;

	for (auto& [stackName, stack] : this->EntWorld.EntMemory)
	{
		auto Block = stack->Top;

		while (Block)
		{
			for (int idx : FRange(0, Block->NumEntities))
			{
				Entity entity = Block->controlArray[idx];

				if (entity.GetName() == name)
				{
					result.Add(entity);
				}
			}

			Block = Block->Next;
		}
	}

	return result;
}

void Scene::FinishUpdate()
{
	//EntWorld.CopyEntMemory();
}

CharacterBody Scene::CreateCharacterBody(FTransform transorm)
{
	return Physics->CreateCharacter(transorm);
}

RigidBody Scene::CreateRigidBody(FTransform transform)
{
	return Physics->CreateDynamic(transform);
}

StaticRigidBody Scene::CreateStaticRigidBody(FTransform transform)
{
	return Physics->CreateStatic(transform);
}

TriangleMeshGeometry Scene::CookTriangleMeshGeometry(PhysicsTriangleMeshDesc desc)
{
	return Physics->CookTriangleMeshGeometry(desc);
}



Scene::~Scene()
{

}


PhysicsAllocator* Scene::PhysicsWorld() { return  this->Physics; }