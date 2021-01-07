#pragma once


#include "Core/Common/CoreBase.h"


template<typename _Type>
struct _FRemoveReference
{
	typedef _Type RefRemovedType;
};

template<typename _Type>
struct _FRemoveReference<_Type&>
{
	typedef _Type RefRemovedType;
};

template<typename _Type>
struct _FRemoveReference<_Type&&>
{
	typedef _Type RefRemovedType;
};


template<typename _Type>
using FRemoveReference = typename _FRemoveReference<_Type>::RefRemovedType;





struct NonCopyable
{
public:
	NonCopyable() {}
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

struct NonMovable
{
public:
	NonMovable() {}
	NonMovable(NonMovable&&) = delete;
	void operator=(NonMovable&&) = delete;
};


struct NonDefaultConstructable
{
public:
	NonDefaultConstructable() = delete;
};

template<typename _Type>
FORCEINLINE FRemoveReference<_Type>&& MoveRef(_Type&& userType)
{
	return static_cast<FRemoveReference<_Type>&&>(userType);
}