#include "GameEntity.h"
#include "../Scene.h"

GameEntity::GameEntity(STRING _modelName)
{

	AssetManager::LoadModel(_modelName, &model);

	Rotation = fQuaternion::Identity();

}

void GameEntity::Render()
{


	fMatrix4 rot = fMatrix4::FromQuaternion(Rotation) * fMatrix4::Scaling(Scale);

	model.Render(fMatrix4::Translation(Position) * rot, _scene);

}


void GameEntity::Update(FrameTime* _frameTime)
{
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
