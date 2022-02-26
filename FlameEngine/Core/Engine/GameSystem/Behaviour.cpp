#include "Behaviour.h"
#include "Common/Scene.h"

Entity& BehaviourScript::AttachedEntity()
{
	return *pEntity;
}

const Entity& BehaviourScript::AttachedEntity() const
{
	return *pEntity;
}

Scene& BehaviourScript::CurrentScene()
{
	return *pScene;
}
const Scene& BehaviourScript::CurrentScene() const
{
	return *pScene;
}
