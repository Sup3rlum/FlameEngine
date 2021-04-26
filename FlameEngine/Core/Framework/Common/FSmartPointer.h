#pragma once

#include "Core/Common/CoreBase.h"
#include "Core/Common/TypeTraits.h"




template<typename TUserType>
EXPORT(struct, FUniquePointer) : public NonCopyable
{
public:

	FUniquePointer()
	{
		data = NULL;
	}

	FUniquePointer(TUserType* ptr)
	{
		data = ptr;
	}

	FUniquePointer(FUniquePointer&& fpOther) noexcept
	{
		this->data = fpOther.data;
		fpOther.data = NULL;
	}

	FUniquePointer& operator=(FUniquePointer&& fpOther)
	{
		this->data = fpOther.data;
		fpOther.data = NULL;

		return *this;
	}


	template<typename...TArgs>
	static FUniquePointer<TUserType> New(TArgs... args)
	{
		return FUniquePointer<TUserType>(new TUserType(args...));
	}

	~FUniquePointer()
	{
		delete data;
	}


	const TUserType* operator->() const
	{
		return data;
	}

	TUserType* operator->()
	{
		return data;
	}

	TUserType& operator*()
	{
		return *data;
	}

	const TUserType& operator*() const
	{
		return *data;
	}



	FORCEINLINE bool IsValid() const
	{
		return data != NULL;
	}
private:
	TUserType* data;
};


template<typename TUserType>
EXPORT(struct, FSharedPointer)
{

public:

	FSharedPointer()
	{
		data = NULL;
		refCounter = new uint64_t();
		*refCounter = 1;
	}

	FSharedPointer(TUserType* ptr)
	{
		data = ptr;
		refCounter = new uint64_t();
		*refCounter = 1;
	}


	// Copy


	FSharedPointer(const FSharedPointer& fpOther)
	{
		this->data = fpOther.data;
		this->refCounter = fpOther.refCounter;

		(*refCounter)++;
	}

	FSharedPointer& operator=(const FSharedPointer& fpOther)
	{

		(*refCounter)--;

		this->data = fpOther.data;
		this->refCounter = fpOther.refCounter;

		(*refCounter)++;
	}

	// Move 

	FSharedPointer(FSharedPointer&& fpOther) noexcept
	{
		this->data = fpOther.data;
		this->refCounter = fpOther.refCounter;

		fpOther.data = NULL;
		fpOther.refCounter = NULL;
	}

	FSharedPointer& operator=(FSharedPointer&& fpOther)
	{
		this->data = fpOther.data;
		this->refCounter = fpOther.refCounter;

		fpOther.data = NULL;
		fpOther.refCounter = NULL;
	}


	template<typename...TArgs>
	static FSharedPointer<TUserType> New(TArgs... args)
	{
		return FSharedPointer<TUserType>(new TUserType(args...));
	}

	~FSharedPointer()
	{
		(*refCounter)--;

		if (*refCounter < 1)
		{
			delete data;
			delete refCounter;
		}
	}


	TUserType* operator->() const
	{
		return data;
	}

	FORCEINLINE uint64_t GetRefCount() const
	{
		return *refCounter;
	}

	FORCEINLINE bool IsValid() const
	{
		return data != NULL && *refCounter > 0;
	}
private:
	TUserType* data;
	uint64_t* refCounter;

};