#include "Entity.h"




bool GEntityID::IsValid()
{
	return HighPtr != nullptr;
}

Entity GEntityID::GetControl()
{
	if (IsValid())
		return Block->controlArray[Index];

	return Entity();
}

Entity& GEntityID::GetControlRef()
{
	return Block->controlArray[Index];
}


FORCEINLINE const FEntityArchetype& Entity::GetArchetype() const
{
	return MemBlock()->Parent->BlockArchetype;
}


void Entity::Kill()
{
	if (EntityId.IsValid())
	{
		auto& archetype = this->GetArchetype();




		MemBlock()->Parent->FreeEntity(*EntityId);
	}
}

uint32 Entity::GetRefCount() const
{
	return EntityId.GetRefCount();
}

Entity::~Entity()
{
	
}

FString Entity::GetName() const
{
	return *Name;
}

bool Entity::IsValid() const
{
	return EntityId.IsValid() && EntityId->IsValid();
}


Entity Entity::GetParent() const
{
	if (ParentId.IsValid())
	{
		return ParentId->GetControl();
	}
	return Entity();
}


FTransform& Entity::Transform()
{
	return Component<FTransform>();
}

const FTransform& Entity::Transform() const
{
	return Component<FTransform>();
}

TransformMode& Entity::TransformMode()
{
	return *_TransformMode;
}

const TransformMode& Entity::TransformMode() const
{
	return *_TransformMode;
}


FTransform Entity::GlobalTransform() const
{
	if (TransformMode() == TransformMode::Relative)
	{
		if (ParentId.IsValid() && ParentId->IsValid())
		{
			return FTransform(Transform().GetMatrix() * GetParent().GlobalTransform().GetMatrix());
		}
		else
		{
			return Transform();
		}
	}

	return Transform();
}

const FArray<struct Entity>& Entity::Children() const
{
	return *_Children;
}
FArray<struct Entity>& Entity::Children()
{
	return *_Children;
}


void Entity::AddChild(Entity child)
{
	if (!child.GetParent().IsValid())
	{
		*child.ParentId = *this->EntityId;
		Children().Add(child);
	}
}

Entity::Entity() :
	EntityId(new GEntityID{0,0}),
	ParentId(new GEntityID{0,0}),
	Name(new FString("")),
	_Children(new FArray<Entity>()),
	_TransformMode(new ::TransformMode{})
{
}

//private:

Entity::Entity(GEntityID* eid, const FString & name) :
	EntityId(eid),
	Name(new FString(name)),
	ParentId(new GEntityID{ 0,0 }),
	_Children(new FArray<Entity>()),
	_TransformMode(new ::TransformMode{})
{
}
Entity::Entity(Entity&& other) noexcept :
	EntityId(MoveRef(other.EntityId)),
	ParentId(MoveRef(other.ParentId)),
	Name(MoveRef(other.Name)),
	_Children(MoveRef(other._Children)),
	_TransformMode(MoveRef(other._TransformMode))
{
}
Entity::Entity(const Entity& other) :
	EntityId(other.EntityId),
	ParentId(other.ParentId),
	Name(other.Name),
	_Children(other._Children),
	_TransformMode(other._TransformMode)
{

}

Entity& Entity::operator=(Entity&& other) noexcept
{
	EntityId = MoveRef(other.EntityId);
	Name = MoveRef(other.Name);
	ParentId = MoveRef(other.ParentId);
	_Children = MoveRef(other._Children);
	_TransformMode = MoveRef(other._TransformMode);

	return *this;
}
Entity& Entity::operator=(const Entity& other)
{
	EntityId = other.EntityId;
	Name = other.Name;
	ParentId = other.ParentId;
	_Children = other._Children;
	_TransformMode = other._TransformMode;
	
	return *this;
}