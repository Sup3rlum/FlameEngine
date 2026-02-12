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
	typedef FDelegate<void(byte*)> DeallocType;
	typedef FKeyVal<FComponentType, DeallocType> CompDescrType;

	FArray<CompDescrType> ComponentTypes;
	uint32 MemColumnSize;
	uint32 MemAlignment;

	size_t HashCode;

	template<typename TComponent>
	FEntityArchetype& AddComponent()
	{
		/*
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

		MemAlignment = max(MemAlignment, alignof(TComponent));*/

		return *this;
	}

	friend static bool operator==(const FEntityArchetype& left, const FEntityArchetype& right)
	{
		if (left.ComponentTypes.Length() != right.ComponentTypes.Length())
			return false;

		for (int i = 0; i < left.ComponentTypes.Length(); i++)
		{
			if (left.ComponentTypes[i].Key != right.ComponentTypes[i].Key)
				return false;
		}

		return true;
	}

	template<typename TComponent>
	int32 GetIndex() const
	{
		FComponentType cmp = TComponentType<TComponent>();

		for (int i = 0; i < ComponentTypes.Length(); i++)
		{
			if (cmp == ComponentTypes[i].Key)
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
	FEntityArchetype(uint32 columnByteSize, uint32 columnAlignment);
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

	template<typename TComponent>
	DeallocType CreateDealloc()
	{
		// If component has Release() method, add it to the deallocator
		constexpr bool has_Release = requires(TComponent comp)
		{
			comp.Release();
		};
		if constexpr (has_Release)
		{
			return DeallocType::Make([=](byte* pData) { ((TComponent*)pData)->Release(); });
		}
		return DeallocType::Make([=](byte* pData) {});
	}

	TEntityArchetype() :
		FEntityArchetype((sizeof(TComponentArgs) + ...), _Flame_CExpr_MulAlignof<TComponentArgs...>())
	{
		constexpr uint32 componentNum = sizeof...(TComponentArgs);

		/* Create an array of the component type descriptions and sort it by ID */

		CompDescrType* compTypes = new CompDescrType[]{ CompDescrType(TComponentType<TComponentArgs>(), CreateDealloc<TComponentArgs>()) ...};
		Sort::Insertion(compTypes, componentNum, [](CompDescrType& componentType) -> typename FComponentType::_InternalId&
			{
				return componentType.Key._TypeId;
			});

		//this->NumComponentTypes = componentNum;
		this->ComponentTypes = FArray<CompDescrType>(compTypes, componentNum);

		/* Compute Hash code */

		HashCode = 0;
		for (int i = 0; i < ComponentTypes.Length(); i++)
		{
			HashCode += ComponentTypes[i].Key._TypeId;
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
