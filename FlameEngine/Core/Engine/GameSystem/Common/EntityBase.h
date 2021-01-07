#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"
#include "ThinkingObject.h"
#include "Core/Framework/Globals/FGlobalID.h"

/*
*	Base Entity	Class for all Scene Objects
* 
*	Contains an interface, for thinking (updating), grouping, ref tracking, naming, serialization etc
* 
* 
*/



EXPORT(class, FSceneEntityBase) : public ThinkingObject
{
protected:
	FTransform Transform;
	FGlobalID ID;

public:

	virtual ~FSceneEntityBase() {}
	
	FGlobalID GetID() { return ID; }

	virtual void Update();

	virtual FSceneEntityBase* Clone() {}

	FORCEINLINE FVector3 GetPosition() const
	{
		return Transform.Position;
	}
	FORCEINLINE FQuaternion GetOrientation() const
	{
		return Transform.Orientation;
	}
	FORCEINLINE FTransform GetTransform() const
	{
		return Transform;
	}

};

template<typename TGenEntityType>
EXPORT(class,  FSceneEntity) : public FSceneEntityBase
{


public:
	FSceneEntity()
	{
	}
	FSceneEntity(FString name)
	{
		if (name.IsEmpty())
		{
			FLAME_WARNING("Assigning null names makes entities unsearchable!");
			return;
		}

		this->ID = name;
	}

	typedef TGenEntityType EntityType;
};

