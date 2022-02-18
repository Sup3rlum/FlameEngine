#include "Entity.h"




bool GEntityID::IsValid()
{
	return HighPtr != nullptr;
}

FORCEINLINE FEntityArchetype Entity::GetArchetype() const
{
	return EntityId->Block->Parent->BlockArchetype;
}

void Entity::Delete()
{
	if (EntityId)
	{
		EntityId->Block->Parent->FreeEntity(*EntityId);
	}
}

void Entity::Invalidate()
{
	delete EntityId;
	delete refCount;

}

uint32 Entity::GetRefCount() const
{
	return *refCount;
}

Entity::~Entity()
{
	if (refCount)
	{
		(*refCount)--;

		/*if (*refCount == 0)
		{
			Invalidate();
		}*/
	}
}

FString Entity::Name() const
{
	return name;
}

bool Entity::IsValid() const
{
	bool idIsvalid = EntityId->IsValid();

	return EntityId != nullptr && refCount != nullptr && *refCount > 0 && idIsvalid;
}

Entity::Entity() :
	EntityId(NULL),
	refCount(0),
	name("")
{
}

//private:

Entity::Entity(GEntityID * eid, const FString & name) :
	EntityId(eid),
	refCount(new uint32(1)),
	name(name)
{
}
Entity::Entity(Entity&& other) noexcept :
	EntityId(other.EntityId),
	refCount(other.refCount),
	name(MoveRef(other.name))
{
}
Entity::Entity(const Entity& other)
{
	EntityId = other.EntityId;
	refCount = other.refCount;
	name = other.name;

	(*refCount)++;
}

Entity& Entity::operator=(Entity&& other) noexcept
{
	EntityId = other.EntityId;
	refCount = other.refCount;
	name = MoveRef(other.name);

	return *this;
}
void Entity::operator=(const Entity& other)
{
	EntityId = other.EntityId;
	refCount = other.refCount;
	name = other.name;

	(*refCount)++;
}