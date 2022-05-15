#include "Scene.h"
#include "SceneSystems.h"
#include "Core/Math/Geometry/Volumes/AABB.h"


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


Scene::Scene(PhysicsSceneDescription desc, FRIContext* renderContext) : 
	Physics(desc.pAllocator),
	physicsScene(desc.pScene),
	physicsService(desc.pService),
	FriContext(renderContext)
{

	uxContainer = new UXContainer(FriContext);
}


void Scene::LoadSystems()
{
}

#define PROFILE_(name, a, code) auto now_##a = FTime::GetTimestamp(); \
							code \
							a += FTime::GetTimestamp() - now_##a; \


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
}

void Scene::Update(FGameTime gameTime)
{

	PROFILE_("Physics", physTime,
		uxContainer->UpdateContainer();
		physicsScene->Step(gameTime.DeltaTime.GetSeconds());
	)

	PROFILE_("Behaviour", behTime,
		UpdateBehaviour(gameTime);
	)

	PROFILE_("Dyn", dynTime,
		UpdateSystems();
	)

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

				for (int i = 0; i < SM_CASCADES; i++)
				{
					SplitFrustum(frustumCorners, frustumSplitCorners, SM_CASCADES, i);

					FViewMatrix viewMatrix(FVector3(0), dirLight.Direction, FVector3(0, 1, 0));

					FMatrix3 toGlobalSpace = FMatrix4::ToMatrix3(viewMatrix);
					FMatrix3 toLocalSpace = FMatrix3::Transpose(toGlobalSpace);

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
					dirLight.FrustumInfo[i].View = FViewMatrix(position, position + dirLight.Direction, aabbUp);
					dirLight.FrustumInfo[i].Projection = FOrthographicMatrix(-halfLengthX, halfLengthX, -halfLengthY, halfLengthY, 0.0f, aabb.LengthZ() + cascadeBias);

					float zFar = 500.0f;
					float zNear = 0.1f;

					dirLight.FrustumInfo[i].Depth = (zFar - zNear) * ((float)(i + 1) / (float)SM_CASCADES) + zNear;
				}
			}
		});
}


AABB Scene::GetAABB() const
{
	if (SceneLevel.LevelGeometry.Root)
		return SceneLevel.LevelGeometry.Root->BoundingBox;

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
	EntWorld.CopyEntMemory();
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
