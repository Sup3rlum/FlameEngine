#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Framework/Algorithm/Sort.h"
#include "Core/Framework/Algorithm/Algorithm.h"
#include <typeinfo>


template<typename ...TArgs>
constexpr size_t SumSizes()
{
	return (sizeof(TArgs) + ...);
}

#define ENTITY_STAGE(...)					\
constexpr static size_t GetStageMemorySize()	\
{											\
	return SumSizes<__VA_ARGS__>();			\
};											\


EXPORT(struct, FComponentType)
{
	typedef size_t _InternalId;
	_InternalId _TypeId;

	const char* _DecoratedName;

	FComponentType();
	uint64 SizeOf() const;

	friend static bool operator==(const FComponentType & left, const FComponentType & right)
	{
		return left._TypeId == right._TypeId;
	}
	friend static bool operator!=(const FComponentType & left, const FComponentType & right)
	{
		return !(left == right);
	}


	FComponentType(const FComponentType & other);
	FComponentType& operator=(const FComponentType & other);

protected:
	uint64 _Size;
	FComponentType(_InternalId id, uint64 size, const char* name);
	friend class FEntityArchetype;
};

template<typename TComponent>
struct TComponentType : public FComponentType
{
	TComponentType() : 
		FComponentType(typeid(TComponent).hash_code(), sizeof(TComponent), typeid(TComponent).name())
	{
	}
};


EXPORT(struct, FEntityArchetype)
{
	template<typename TComponent>
	struct FComponentIndexer
	{
		inline static int32 index = -1;
	};

	FComponentType* ComponentTypes;
	uint32 NumComponentTypes;
	uint32 MemColumnSize;
	uint32 MemAlignment;

	size_t HashCode;

	template<typename TComponent>
	FEntityArchetype& AddComponent()
	{
		// Resize Array
		FComponentType* compTypes = new FComponentType[NumComponentTypes + 1];
		Memory::CopyArray(compTypes, ComponentTypes, NumComponentTypes);
		compTypes[NumComponentTypes] = TComponentType<TComponent>();

		// Sort Component types by ID
		Sort::Insertion(compTypes, NumComponentTypes++, [](FComponentType& componentType) -> typename FComponentType::_InternalId&
			{
				return componentType._TypeId;
			});


		delete ComponentTypes;
		ComponentTypes = compTypes;
		MemColumnSize += sizeof(TComponent);

		MemAlignment = max(MemAlignment, alignof(TComponent));

		return *this;
	}

	friend static bool operator==(const FEntityArchetype& left, const FEntityArchetype& right)
	{
		if (left.NumComponentTypes != right.NumComponentTypes)
			return false;

		for (int i = 0; i < left.NumComponentTypes; i++)
		{
			if (left.ComponentTypes[i] != right.ComponentTypes[i])
				return false;
		}

		return true;
	}

	template<typename TComponent>
	int32 GetIndex() const
	{
		FComponentType cmp = TComponentType<TComponent>();

		for (int i = 0; i < NumComponentTypes; i++)
		{
			if (cmp == ComponentTypes[i])
			{
				return i;
			}
		}

		return -1;
	}

	bool Contains(const FEntityArchetype& type);

	FEntityArchetype(const FEntityArchetype& archetypeCopy);
	~FEntityArchetype();
protected:
	FEntityArchetype(uint32 numComponents, FComponentType* cds, uint32 columnByteSize, uint32 columnAlignment);
	FEntityArchetype() = delete;

};

template<typename TFirst, typename...TArgs>
constexpr uint64 _Flame_CExpr_MulAlignof()
{
	constexpr int tailAlign[] = { alignof(TFirst), alignof(TArgs) ... };
	uint64 maxVal = tailAlign[0];
	for (uint32 i = 0; i < sizeof...(TArgs); i++)
		maxVal = max(maxVal, tailAlign[i]);
	return maxVal;
}


template<typename...TComponentArgs>
struct TEntityArchetype : public FEntityArchetype
{
	TEntityArchetype() :
		FEntityArchetype(0, NULL, (sizeof(TComponentArgs) + ...), _Flame_CExpr_MulAlignof<TComponentArgs...>())
	{
		constexpr uint32 componentNum = sizeof...(TComponentArgs);

		/* Create an array of the component type descriptionsand sort it by ID */
		/* This makes algorithms on archetypes*/

		FComponentType* compTypes = new FComponentType[]{ TComponentType<TComponentArgs>() ... };

		Sort::Insertion(compTypes, componentNum, [](FComponentType& componentType) -> typename FComponentType::_InternalId&
			{
				return componentType._TypeId;
			});

		this->NumComponentTypes = componentNum;
		this->ComponentTypes = compTypes;

		/* Compute Hash code */

		HashCode = 0;
		for (int i = 0; i < NumComponentTypes; i++)
		{
			HashCode += ComponentTypes[i]._TypeId;
		}
	}
};


struct FComponentTypeHasher
{
	size_t operator ()(const FComponentType& comp) const
	{
		return (size_t)comp._TypeId;
	}
};
