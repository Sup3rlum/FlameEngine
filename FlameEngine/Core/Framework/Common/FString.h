#pragma once

#include <cstring>
#include "Core/Common/CoreBase.h"
#include "FArray.h"
#include "FStringMemoryAllocator.h"



/*
*  Defines an immutable, stack allocated string
*/

template<typename TBasicChar, size_t GenSize>
struct TStaticString: public FStaticArray<TBasicChar, GenSize>
{
public:
	TStaticString()
	{
	}


	template<typename TBasicType>
	TStaticString(TBasicType val)
	{
		size_t tsize = FBaseStringMemoryAllocator::GetLength(val);
		FBaseStringMemoryAllocator::Allocate(this->dataInternal, val, min(tsize, GenSize));
		this->dataInternal[tsize] = 0;
	}

	size_t GetStrLen() const
	{
		return FBaseStringMemoryAllocator::GetLength(this->dataInternal);
	}

	const TBasicChar* ToPlatformString() const
	{
		return this->dataInternal;
	}
};

/*
*  Defines an dynamic string
*/

template<typename TBaseChar>
struct TString : public FArray<TBaseChar>
{
	typedef FArray<TBaseChar> BaseType;
	typedef TBaseChar CharType;

public:
	TString() : BaseType() {}
	TString(const TString& f) : BaseType(f) {}
	TString(TString&& f) noexcept : BaseType(f) {}
	
	template<size_t GenSize>
	TString(const TStaticString<TBaseChar, GenSize>& str) : BaseType(str) {}


private:
	template<typename TGenType>
	void TConstructFromType(TGenType val)
	{
		this->Resize(FBaseStringMemoryAllocator::GetLength(val));
		FBaseStringMemoryAllocator::Allocate(this->ptrInternal, val, this->size);
	}

public:

	TString(double val) { TConstructFromType(val); }
	TString(float val) { TConstructFromType(val); }
	TString(int32 val) { TConstructFromType(val); }
	TString(uint32 val) { TConstructFromType(val); }
	TString(int64 val) { TConstructFromType(val); }
	TString(uint64 val) { TConstructFromType(val); }
	TString(const FChar* val) { TConstructFromType(val); }
	TString(const char* val) { TConstructFromType(val); }
	TString(FChar val) { TConstructFromType(val); }
	TString(char val) { TConstructFromType(val); }

	BaseType ToCharArray() const
	{
		return BaseType(this->ptrInternal, this->size);
	}


	TBaseChar* ToPlatformString() const
	{
		TBaseChar* allocation = NULL;
		if (this->ptrInternal)
		{
			allocation = Memory::AllocCounted<TBaseChar>(this->size + 1);
			Memory::CopyCounted(allocation, this->ptrInternal, this->size);
			allocation[this->size] = 0;
		}
		return allocation;
	}



	FArray<int> FindAll(const TString& fstring, const TString& what)
	{
		FArray<int> occurances;

		if (what.Length() > fstring.Length())
			return occurances;


		for (int i = 0; i < fstring.Length(); i++)
		{
			// We have passed the point where there is not enough string left to match
			if (i + what.Length() > fstring.Length())
			{
				break;
			}

			// Instead of returning the first occurance, we add it to the list of found ones
			if (!memcmp(&fstring[i], what.Begin(), what.ByteSize()))
			{
				occurances.Add(i);
			}

		}

		return occurances;
	}


	FArray<TString> Split(TBaseChar delimiter)
	{
		FArray<TString> splits;
		TString tStr;
		for (auto& tChar : *this)
		{
			if (tChar == delimiter)
			{
				splits.Add(tStr);
				tStr = TString();
				continue;
			}

			tStr.Add(tChar);
		}

		splits.Add(tStr);
		return splits;
	}

	//int FindFirst(const TString& fstring, const TString& what);

	// TODO : Implement
	void Replace(const TString& what, const TString& withWhat) {}


	template<typename...TArgs>
	static TString Format(const TString& format, const TArgs& ...);



	TString& operator=(const TString& str)
	{
		BaseType::operator=(str);
		return *this;
	}
	TString& operator=(TString&& str) noexcept
	{
		BaseType::operator=(MoveRef(str));
		return *this;
	}

	TString& operator+=(const TString& other)
	{
		this->AddArray(other);
		return *this;
	}

};





template<size_t GenSize>
using FStaticAnsiString = TStaticString<char, GenSize>;

template<size_t GenSize>
using FStaticString = TStaticString<FChar, GenSize>;

template<size_t GenSize>
using FStaticUTF32String = TStaticString<char32_t, GenSize>;


typedef TString<char> FString8;
typedef TString<FChar> FString;
typedef TString<char32_t> FUTF32String;



enum class EStringByteEncoding
{
	ANSI,
	UTF8
};

struct FStringEncoder
{
	static FString GetFromBytes(FArray<byte> byteArray, EStringByteEncoding encoding);
};


#include "FString.inl"

