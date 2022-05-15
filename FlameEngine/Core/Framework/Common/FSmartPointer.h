#pragma once

#include "Core/Common/CoreBase.h"
#include "Core/Common/TypeTraits.h"




template<typename TUserType>
struct FUniquePtr : public NonCopyable
{
public:

	FUniquePtr()
	{
		data = NULL;
	}

	FUniquePtr(TUserType* ptr)
	{
		data = ptr;
	}

	FUniquePtr(FUniquePtr&& fpOther) noexcept
	{
		this->data = fpOther.data;
		fpOther.data = NULL;
	}

	FUniquePtr& operator=(FUniquePtr&& fpOther)
	{
		this->data = fpOther.data;
		fpOther.data = NULL;

		return *this;
	}


	template<typename...TArgs>
	static FUniquePtr<TUserType> New(TArgs... args)
	{
		return FUniquePtr<TUserType>(new TUserType(args...));
	}

	~FUniquePtr()
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

	operator TUserType*()
	{
		return data;
	}

	operator const TUserType* () const
	{
		return data;
	}



	FORCEINLINE bool IsValid() const
	{
		return data != NULL;
	}
private:
	TUserType* data;
};


template<typename TUserType>
struct FSharedPtr
{

public:

	FSharedPtr()
	{
		pData = NULL;
		RefCounter = new uint64_t(0);
	}

	FSharedPtr(TUserType* ptr)
	{
		pData = ptr;
		RefCounter = new uint64_t(1);
	}

	/* Copy */

	FSharedPtr(const FSharedPtr& fpOther)
	{
		this->pData = fpOther.pData;
		this->RefCounter = fpOther.RefCounter;

		Incr();
	}

	FSharedPtr& operator=(const FSharedPtr& fpOther) noexcept
	{
		Decr();

		this->pData = fpOther.pData;
		this->RefCounter = fpOther.RefCounter;

		Incr();

		return *this;
	}

	// Move 

	FSharedPtr(FSharedPtr&& fpOther) noexcept
	{
		this->pData = fpOther.pData;
		this->RefCounter = fpOther.RefCounter;

		fpOther.pData = NULL;
		fpOther.RefCounter = NULL;
	}

	FSharedPtr& operator=(FSharedPtr&& fpOther)
	{
		Decr();

		this->pData = fpOther.pData;
		this->RefCounter = fpOther.RefCounter;

		fpOther.pData = NULL;
		fpOther.RefCounter = NULL;

		return *this;
	}


	template<typename...TArgs>
	static FSharedPtr<TUserType> New(TArgs... args)
	{
		return FSharedPtr<TUserType>(new TUserType(args...));
	}

	~FSharedPtr()
	{
		Decr();
	}


	TUserType* operator->() const
	{
		return pData;
	}

	TUserType& operator*() const
	{
		return *pData;
	}


	FORCEINLINE uint64_t GetRefCount() const
	{
		return *RefCounter;
	}

	FORCEINLINE bool IsValid() const
	{
		return RefCounter != NULL && pData != NULL && Ctr() > 0;
	}
private:

	void Incr() { Ctr()++; }

	void Decr()
	{
		if (!RefCounter)
			return;

		if (Ctr() <= 1)
		{
			delete pData;
			delete RefCounter;

			pData = NULL;
			RefCounter = NULL;

			return;
		}

		Ctr()--;
	}

	uint64_t& Ctr()
	{
		return *RefCounter;
	}
	const uint64_t& Ctr() const
	{
		return *RefCounter;
	}
	TUserType* pData;
	uint64_t* RefCounter;

};



template<typename TUserType>
struct FRefPtr
{

public:

	FRefPtr()
	{
		pData = NULL;
		RefCounter = new uint64_t(0);
	}

	FRefPtr(TUserType* ptr)
	{
		pData = ptr;
		RefCounter = new uint64_t(1);
	}

	/* Copy */

	FRefPtr(const FRefPtr& fpOther)
	{
		this->pData = fpOther.pData;
		this->RefCounter = fpOther.RefCounter;

		Incr();
	}

	FRefPtr& operator=(const FRefPtr& fpOther) noexcept
	{
		Decr();

		this->pData = fpOther.pData;
		this->RefCounter = fpOther.RefCounter;

		Incr();

		return *this;
	}

	// Move 

	FRefPtr(FRefPtr&& fpOther) noexcept
	{
		this->pData = fpOther.pData;
		this->RefCounter = fpOther.RefCounter;

		fpOther.pData = NULL;
		fpOther.RefCounter = NULL;
	}

	FRefPtr& operator=(FRefPtr&& fpOther)
	{
		Decr();

		this->pData = fpOther.pData;
		this->RefCounter = fpOther.RefCounter;

		fpOther.pData = NULL;
		fpOther.RefCounter = NULL;

		return *this;
	}

	/*
	FRefPtr& operator=(TUserType*&& fpOther)
	{
		Decr();

		this->pData = fpOther;
		this->RefCounter = new uint64(1);

		return *this;
	}*/


	template<typename...TArgs>
	static FRefPtr<TUserType> New(TArgs... args)
	{
		return FRefPtr<TUserType>(new TUserType(args...));
	}

	~FRefPtr()
	{
		Decr();
	}


	TUserType* operator->() const
	{
		return pData;
	}

	TUserType& operator*() const
	{
		return *pData;
	}


	FORCEINLINE uint64_t GetRefCount() const
	{
		return *RefCounter;
	}

	FORCEINLINE bool IsValid() const
	{
		return RefCounter != NULL && pData != NULL && Ctr() > 0;
	}

	operator TUserType*()
	{
		return pData;
	}

	operator const TUserType* () const
	{
		return pData;
	}

private:

	void Incr() { Ctr()++; }

	void Decr()
	{
		if (!RefCounter || !Ctr())
			return;

		if (Ctr() <= 1)
		{
			delete pData;
			delete RefCounter;

			pData = NULL;
			RefCounter = NULL;

			return;
		}

		Ctr()--;
	}

	uint64_t& Ctr()
	{
		return *RefCounter;
	}
	const uint64_t& Ctr() const
	{
		return *RefCounter;
	}
	TUserType* pData;
	uint64_t* RefCounter;

};