#pragma once

#include "EntityComponent/EntityComponent.h"
#include "Core/Math/Module.h"

struct TransformComponent
{
	FTransform Transform;
	
	TransformComponent(const FTransform& tr) :
		Transform(tr)
	{

	}

	TransformComponent(const TransformComponent& other) :
		Transform(other.Transform)
	{
	}
};

