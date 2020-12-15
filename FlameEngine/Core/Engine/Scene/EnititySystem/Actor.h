#pragma once


#include "../../dll/nchfx.h"

#include "../../Graphics/ShaderDefinitions/Shader.h"
#include "RenderableObject.h"
#include "UpdatableObject.h"
#include "../Physics/PhysXService.h"


EXPORT_FORWARD_DECL(class,  Scene);

EXPORT(class,  Actor) : public RenderableObject, public UpdatableObject
{
public:
	
	uint32_t id;

	Actor();

	FVector3 Position;
	FQuaternion Rotation;
	FVector3 Scale;


	Scene* _scene;


	PxRigidActor* pPxActor;

};

