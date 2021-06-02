#include "Scene.h"
#include "SceneSystems.h"


#include "Core/Math/Geometry/Volumes/AABB.h"



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


Scene::Scene(PhysicsSceneDescription desc) : 
	sceneID("someScene"),
	Physics(desc.pAllocator),
	physicsScene(desc.pScene),
	physicsService(desc.pService)
{

}


void Scene::LoadSystems()
{
	RegisterSystem<DynamicPhys>();
	RegisterSystem<AnimSystem>();
}


void Scene::Update()
{

	UpdateFrameNum++;

	auto timestamp = FTimeStamp::MarkCurrent();
	FrameTimestamp = timestamp.data;

	double FrameDelta = FrameTimestamp - LastFrameTimestamp;
	FrameDelta /= (double)FTime::PlatformTickFrequency();

	LastFrameTimestamp = FrameTimestamp;

	physicsScene->Step((float)FrameDelta);


	for (auto sysPtr : Systems)
	{
		sysPtr->Tick();
	}


	CameraComponent& camRef = Camera.Component<CameraComponent>();
	DirectionalLightComponent& dirLightRef = Sun.Component<DirectionalLightComponent>();


	FStaticArray<FVector3, 8> frustumCorners;
	FStaticArray<FVector3, 8> frustumSplitCorners;
	{
		// Create an orthonormal basis describing the light's local coordinate system

		camRef.GetFrustumCorners(frustumCorners);


		for (int i = 0; i < 5; i++)
		{

			SplitFrustum(frustumCorners, frustumSplitCorners, 5, i);


			FViewMatrix viewMatrix(FVector3(0), dirLightRef.Direction, FVector3(0, 1, 0));

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

			// Get the position of the camera as being in the middle of the +Z plane of the AABB and turning into global space
			FVector3 position = toGlobalSpace * (aabb.Center() + FVector3(0, 0, aabb.LengthZ() / 2.0f));


			float halfLengthX = aabb.LengthX() / 2.0f;
			float halfLengthY = aabb.LengthY() / 2.0f;


			//Get the up component required to reorient the AABB into global space
			FVector3 aabbUp = toGlobalSpace * FVector3(0, 1, 0);

			// Create the view and projection matrices for the light's camera that envelops the user view frustum
			dirLightRef.FrustumInfo[i].View = FViewMatrix(position, position + dirLightRef.Direction, aabbUp);
			dirLightRef.FrustumInfo[i].Projection = FOrthographicMatrix(-halfLengthX, halfLengthX, -halfLengthY, halfLengthY, 0.0f, aabb.LengthZ());

			/*
			dirLightRef.FrustumInfo[i].ViewToLight = dirLightRef.FrustumInfo[i].View * FMatrix4::Inverse(camRef.View);
			dirLightRef.FrustumInfo[i].ViewToLight = dirLightRef.FrustumInfo[i].Projection * dirLightRef.FrustumInfo[i].ViewToLight;*/

			float zFar = 300.0f;
			float zNear = 0.1f;

			dirLightRef.FrustumInfo[i].Depth.r = (zFar - zNear) * ((float)(i+1) / 5.0f) + zNear;
		}
	}
}

Scene::~Scene()
{

}