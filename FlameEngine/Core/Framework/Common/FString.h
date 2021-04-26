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



template<typename TBaseChar>
EXPORT(struct, TString) : public FArray<TBaseChar>
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

	BaseType ToCharArray() const
	{
		return BaseType(this->ptrInternal, this->size);
	}


	TBaseChar* ToPlatformString() const
	{
		TBaseChar* allocation = Memory::AllocCounted<TBaseChar>(this->size + 1);
		Memory::CopyCounted(allocation, this->ptrInternal, this->size);
		allocation[this->size] = 0;

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
	//int FindFirst(const TString& fstring, const TString& what);

	// TODO : Implement
	void Replace(const TString& what, const TString& withWhat) {}



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


template<typename TBasicChar>
EXPORT(struct, TStringFormatArg)
{
	enum EType { Int, UInt, Double, String, StringLiteral };

	EType Type;

	union
	{
		int64_t IntValue;
		uint64_t UIntValue;
		double DoubleValue;
		const TBasicChar* StringLiteralValue;
	};

	TString<TBasicChar> StringValue;

	TStringFormatArg(const int32_t Value) : Type(Int), IntValue(Value) {}
	TStringFormatArg(const uint32_t Value) : Type(UInt), UIntValue(Value) {}
	TStringFormatArg(const int64_t Value) : Type(Int), IntValue(Value) {}
	TStringFormatArg(const uint64_t Value) : Type(UInt), UIntValue(Value) {}
	TStringFormatArg(const float Value) : Type(Double), DoubleValue(Value) {}
	TStringFormatArg(const double Value) : Type(Double), DoubleValue(Value) {}
	TStringFormatArg(TString<TBasicChar> Value) : Type(String), StringValue(Value) {}
	TStringFormatArg(const TBasicChar * Value) : Type(StringLiteral), StringLiteralValue(Value) {}
	TStringFormatArg(const TStringFormatArg & RHS)
	{
		Type = RHS.Type;
		switch (Type)
		{
		case Int: 				IntValue = RHS.IntValue; break;
		case UInt: 				UIntValue = RHS.UIntValue; break;
		case Double: 			IntValue = RHS.IntValue; break;
		case String: 			StringValue = RHS.StringValue; break;
		case StringLiteral: 	StringLiteralValue = RHS.StringLiteralValue; break;
		}
	}


private:

	TStringFormatArg();
};


template<size_t GenSize>
using FStaticAnsiString = TStaticString<char, GenSize>;

template<size_t GenSize>
using FStaticString = TStaticString<FChar, GenSize>;

template<size_t GenSize>
using FStaticUTF32String = TStaticString<char32_t, GenSize>;


typedef TString<char> FAnsiString;
typedef TString<FChar> FString;
typedef TString<char32_t> FUTF32String;



template<typename TBasicChar>
EXPORT(struct, TStringFormatter)
{
	typedef TString<TBasicChar> TStringType;
	typedef TStringFormatArg<TBasicChar> TArgType;

public:
	template<typename...TArgs>
	static TStringType Format(const TStringType& Format, const TArgs&... args)
	{
		FArray<TArgType> formatargs = { TArgType(args) ... };

		TStringType result;

		int formatReq = 0;

		for (size_t i = 0; i < Format.Length(); i++)
		{
			if (Format[i] != FStringFormatterSpecifier<TBasicChar>::Percent())
			{
				result += Format[i];
			}
			else
			{

				int formatIndex = (Format[i++ + 1] - FStringFormatterSpecifier<TBasicChar>::Zero() /* "0" */);

				if (Format[i] == FStringFormatterSpecifier<TBasicChar>::Percent())
				{
					result += FStringFormatterSpecifier<TBasicChar>::Percent() /* % */;
				}

				if (formatIndex >= 0 && formatIndex <= 9)
				{
					typename TArgType::EType debugt = formatargs[formatIndex].Type;

					if (debugt == TArgType::Double)
					{
						result += TStringType(formatargs[formatIndex].DoubleValue);
					}
					if (debugt == TArgType::Int)
					{
						result += TStringType(formatargs[formatIndex].IntValue);
					}
					if (debugt == TArgType::UInt)
					{
						result += TStringType(formatargs[formatIndex].UIntValue);
					}
					if (debugt == TArgType::StringLiteral)
					{
						result += TStringType(formatargs[formatIndex].StringLiteralValue);
					}
					if (debugt == TArgType::String)
					{
						result += formatargs[formatIndex].StringValue;
					}
				}
			}
		}

		return result;
	}

};

typedef TStringFormatter<char> FAnsiStringFormatter;
typedef TStringFormatter<FChar> FStringFormatter;



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

