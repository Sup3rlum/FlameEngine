#pragma once


#include "Core/Engine/FlameRI/FRI.h"
#include "../Material/MaterialComponent.h"
#include "Core/Framework/Algorithm/BVH.h"
#include "../Physics/PhysicsComponent.h"


struct StaticGeometry
{
	FRIVertexBuffer* VertexBuffer;
	FRIIndexBuffer* IndexBuffer;

	Material Material;
	StaticRigidBody PhysicsBody;

	StaticGeometry(StaticRigidBody rigidBody) :
		PhysicsBody(rigidBody),
		VertexBuffer(NULL),
		IndexBuffer(NULL)
	{}

};


EXPORT(class, Level)
{
public:
	IBVHierarchy<2, StaticGeometry> LevelGeometry;


	Level() {}

	Level(const Level& other) :
		LevelGeometry(other.LevelGeometry)
	{}

	Level& operator=(const Level& other)
	{
		LevelGeometry = other.LevelGeometry;

		return *this;
	}
};

