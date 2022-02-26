#include "Entity.h"




bool GEntityID::IsValid()
{
	return HighPtr != nullptr;
}

Entity& GEntityID::GetControl()
{
	return Block->controlArray[Index];
}


FORCEINLINE const FEntityArchetype& Entity::GetArchetype() const
{
	return EntityId->Block->Parent->BlockArchetype;
}

/*
void Entity::Delete()
{
	if (EntityId.IsValid())
	{
		EntityId->Block->Parent->FreeEntity(*EntityId);
	}
}*/

uint32 Entity::GetRefCount() const
{
	return EntityId.GetRefCount();
}

Entity::~Entity()
{
	
}

FString Entity::GetName() const
{
	return Name;
}

bool Entity::IsValid() const
{
	return EntityId.IsValid() && EntityId->IsValid();
}

Entity::Entity() :
	EntityId(NULL),
	Name("")
{
}

//private:

Entity::Entity(GEntityID * eid, const FString & name) :
	EntityId(eid),
	Name(name)
{
}
Entity::Entity(Entity&& other) noexcept :
	EntityId(MoveRef(other.EntityId)),
	Name(MoveRef(other.Name))
{
}
Entity::Entity(const Entity& other)
{
	EntityId = other.EntityId;
	Name = other.Name;
}

Entity& Entity::operator=(Entity&& other) noexcept
{
	EntityId = MoveRef(other.EntityId);
	Name = MoveRef(other.Name);

	return *this;
}
void Entity::operator=(const Entity& other)
{
	EntityId = other.EntityId;
	Name = other.Name;
}