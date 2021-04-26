#pragma once


struct PhysicsMaterial
{
	float StaticFriction;
	float DynamicFriction;
	float Restitution;


	PhysicsMaterial(float staticFrction, float dynamicFriction, float restitution) :
		StaticFriction(staticFrction),
		DynamicFriction(dynamicFriction),
		Restitution(restitution)
	{

	}
};