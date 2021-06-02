#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Framework/Algorithm/Sort.h"
#include "Core/Framework/Algorithm/Algorithm.h"
#include <typeinfo>

template<typename>
constexpr uint32 DummyOne() { return 1; }



#define RegisterEntityComponent(componentClass, componentNameString, ...)						\
	template<typename TComponent>																\
	struct ComponentRegistrationDefinition;														\
																								\
	template<>																					\
	struct ComponentRegistrationDefinition<componentClass>										\
	{																							\
		static const FEntityComponentRegistryEntry<componentClass>& registration;				\
	};																							\
	const FEntityComponentRegistryEntry<componentClass>&										\
			ComponentRegistrationDefinition<componentClass>::registration						\
			= FEntityComponentRegistryEntry<componentClass>::Create(componentNameString);		\
																								\
																								\




EXPORT(struct, FComponentType)
{
	//typedef void (*_InternalId)();
	typedef size_t _InternalId;

	_InternalId _TypeId;
	uint64 _Size;

	const char* _DecoratedName;

	FComponentType() :
		_TypeId(NULL),
		_Size(NULL),
		_DecoratedName(NULL)
	{}

	uint64 SizeOf() const;


	friend static bool operator==(const FComponentType& left, const FComponentType& right)
	{
		return left._TypeId == right._TypeId;
	}
	friend static bool operator!=(const FComponentType& left, const FComponentType& right)
	{
		return !(left == right);
	}


	FComponentType(const FComponentType& other) :
		_TypeId(other._TypeId),
		_Size(other._Size),
		_DecoratedName(other._DecoratedName)
	{

	}

	FComponentType& operator=(const FComponentType& other)
	{
		_TypeId = other._TypeId;
		_Size = other._Size;
		_DecoratedName = other._DecoratedName;

		return *this;
	}

protected:
	FComponentType(_InternalId id, uint64 size, const char* name) :
		_TypeId(id),
		_Size(size),
		_DecoratedName(name)
	{}

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


struct FEntityArchetype
{
	template<typename TComponent>
	struct FComponentIndexer
	{
		inline static int index = -1;
	};

	FComponentType* ComponentTypes;
	uint32 NumComponentTypes;
	uint32 MemColumnSize;
	uint32 MemAlignment;

	FEntityArchetype(const FEntityArchetype& archetypeCopy) :
		NumComponentTypes(archetypeCopy.NumComponentTypes),
		MemColumnSize(archetypeCopy.MemColumnSize),
		MemAlignment(archetypeCopy.MemAlignment)
	{
		ComponentTypes = new FComponentType[NumComponentTypes];
		Memory::CopyCounted(ComponentTypes, archetypeCopy.ComponentTypes, NumComponentTypes);
	}


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


	bool Contains(const FEntityArchetype& type)
	{
		return Algorithm::SortedIsSubsetOf(type.ComponentTypes, ComponentTypes, type.NumComponentTypes, NumComponentTypes, [](FComponentType& comp) -> FComponentType::_InternalId& { return comp._TypeId; });
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
	~FEntityArchetype()
	{
		delete ComponentTypes;
	}
protected:
	FEntityArchetype(uint32 numComponents, FComponentType* cds, uint32 columnByteSize, uint32 columnAlignment) :
		NumComponentTypes(numComponents),
		ComponentTypes(cds),
		MemColumnSize(columnByteSize),
		MemAlignment(columnAlignment)
	{}
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
		// Compile time elemnt counting
		constexpr uint32 componentNum = sizeof...(TComponentArgs);// (DummyOne<TComponentArgs>() + ...);

		// Create an array of the component dype descriptions and sort it by ID
		FComponentType* compTypes = new FComponentType[]{ TComponentType<TComponentArgs>() ... };

		Sort::Insertion(compTypes, componentNum, [](FComponentType& componentType) -> typename FComponentType::_InternalId&
			{
				return componentType._TypeId;
			});

		this->NumComponentTypes = componentNum;
		this->ComponentTypes = compTypes;
	}
};




struct FComponentTypeHasher
{
	size_t operator ()(const FComponentType& comp) const
	{
		return (size_t)comp._TypeId;
	}
};


/*
typedef FHashMap<FString, FComponentType> ComponentNameRegistry;
//typedef FHashMap<FComponentType, FArray<FComponentType>, FComponentTypeHasher> ComponentRequirementRegistry;


FORCEINLINE ComponentNameRegistry& GetComponentNameRegistry()
{
	static ComponentNameRegistry nameReg;
	return nameReg;
}


/*FORCEINLINE ComponentRequirementRegistry& GetComponentRequirementRegistry()
{
	static ComponentRequirementRegistry reqReg;
	return reqReg;
}




template<typename Type>
struct FEntityComponentRegistryEntry
{
public:
	//template<typename...TReqArgs>
	static FEntityComponentRegistryEntry<Type>& Create(FString name)
	{
		static FEntityComponentRegistryEntry<Type> instance(name);
		return instance;
	}

private:
	FEntityComponentRegistryEntry(FString name)// , const FArray<FComponentType>& requirementArray)
	{
		FComponentType compType = TComponentType<Type>();
		GetComponentNameRegistry().Set(name, compType);
		//GetComponentRequirementRegistry().Set(compType, requirementArray);

	}
};*/