#include "GameEntity.h"
#include "../Scene.h"

GameEntity::GameEntity(STRING _modelName)
{

	AssetManager::LoadModelFromFile<StaticModel>(_modelName, &model);

	Rotation = FQuaternion::Identity();

}

void GameEntity::Render()
{
	model.Render(_scene);

}


void GameEntity::Update(FrameTime* _frameTime)
{
	FMatrix4 rot = FMatrix4::FromQuaternion(Rotation) * FMatrix4::Scaling(Scale);
	model.World = FMatrix4::Translation(Position) * rot;

	if (pPxActor != NULL)
	{
		Position.x = pPxActor->is<PxRigidActor>()->getGlobalPose().p.x;
		Position.y = pPxActor->is<PxRigidActor>()->getGlobalPose().p.y;
		Position.z = pPxActor->is<PxRigidActor>()->getGlobalPose().p.z;

		Rotation.x = pPxActor->is<PxRigidActor>()->getGlobalPose().q.z;
		Rotation.y = pPxActor->is<PxRigidActor>()->getGlobalPose().q.y;
		Rotation.z = pPxActor->is<PxRigidActor>()->getGlobalPose().q.x;
		Rotation.w = pPxActor->is<PxRigidActor>()->getGlobalPose().q.w;


	}
}
