#include "EntityComponent.h"

size_t FComponentType::SizeOf() const
{
	return _Size;
}


FComponentType::FComponentType() :
	_TypeId(NULL),
	_Size(NULL),
	_DecoratedName(NULL)
{}




FComponentType::FComponentType(const FComponentType& other) :
	_TypeId(other._TypeId),
	_Size(other._Size),
	_DecoratedName(other._DecoratedName)
{

}

FComponentType& FComponentType::operator=(const FComponentType& other)
{
	_TypeId = other._TypeId;
	_Size = other._Size;
	_DecoratedName = other._DecoratedName;

	return *this;
}


FComponentType::FComponentType(_InternalId id, uint64 size, const char* name) :
	_TypeId(id),
	_Size(size),
	_DecoratedName(name)
{}




FEntityArchetype::FEntityArchetype(const FEntityArchetype& archetypeCopy) :
	ComponentTypes(archetypeCopy.ComponentTypes),
	MemColumnSize(archetypeCopy.MemColumnSize),
	MemAlignment(archetypeCopy.MemAlignment),
	HashCode(archetypeCopy.HashCode)
{

}


bool FEntityArchetype::Contains(const FEntityArchetype& type)
{
	return Algorithm::SortedIsSubsetOf
		(
		type.ComponentTypes.Begin(),
		type.ComponentTypes.Length(),

		ComponentTypes.Begin(),
		ComponentTypes.Length(),

		[](CompDescrType& comp) -> FComponentType::_InternalId&
		{ 
			return comp.Key._TypeId; 
		});
}


FEntityArchetype::~FEntityArchetype()
{
	//delete ComponentTypes;
}

FEntityArchetype::FEntityArchetype(uint32 columnByteSize, uint32 columnAlignment) :
	MemColumnSize(columnByteSize),
	MemAlignment(columnAlignment),
	HashCode(0)
{}