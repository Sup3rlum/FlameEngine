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
	NumComponentTypes(archetypeCopy.NumComponentTypes),
	MemColumnSize(archetypeCopy.MemColumnSize),
	MemAlignment(archetypeCopy.MemAlignment)
{
	ComponentTypes = new FComponentType[NumComponentTypes];
	Memory::CopyCounted(ComponentTypes, archetypeCopy.ComponentTypes, NumComponentTypes);
}


bool FEntityArchetype::Contains(const FEntityArchetype& type)
{
	return Algorithm::SortedIsSubsetOf(type.ComponentTypes, ComponentTypes, type.NumComponentTypes, NumComponentTypes, [](FComponentType& comp) -> FComponentType::_InternalId& { return comp._TypeId; });
}


FEntityArchetype::~FEntityArchetype()
{
	delete ComponentTypes;
}

FEntityArchetype::FEntityArchetype(uint32 numComponents, FComponentType* cds, uint32 columnByteSize, uint32 columnAlignment) :
	NumComponentTypes(numComponents),
	ComponentTypes(cds),
	MemColumnSize(columnByteSize),
	MemAlignment(columnAlignment)
{}