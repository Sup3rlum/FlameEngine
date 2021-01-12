#pragma once


#include "Core/Common/CoreBase.h"
#include "FArray.h"
#include <cstring>


typedef wchar_t FChar;
typedef const char* CstrLiteral;
typedef const wchar_t* WCstrLiteral;
typedef char* Cstr;
typedef wchar_t* WCstr;

/*
*  Defines an immutable, stack allocated string
*/


template<size_t GenSize>
struct FStaticANSIString : public FStaticArray<char, GenSize>
{
public:
	FStaticANSIString()
	{
		this->dataInternal = { 0 };
	}
	FStaticANSIString(CstrLiteral str)
	{
		size_t tsize = strlen(str);
		tsize = min(tsize, GenSize);

		memcpy(this->dataInternal, str, tsize * sizeof(char));
		this->dataInternal[tsize] = '\0';
	}


	size_t GetStrLen() const
	{
		return strlen(this->dataInternal);
	}

	FChar* ToCString() const
	{
		return this->dataInternal;
	}
};


template<size_t GenSize>
struct FStaticString : public FStaticArray<FChar, GenSize>
{
public:
	FStaticString()
	{
		this->dataInternal = { 0 };
	}
	FStaticString(CstrLiteral str)
	{
		size_t tsize = strlen(str);
		tsize = min(tsize, GenSize);

		size_t outSize;

		mbstowcs_s(&outSize, this->dataInternal, tsize + 1, str, tsize);

		this->dataInternal[tsize] = '\0';
	}
	FStaticString(WCstrLiteral str)
	{
		size_t tsize = wcslen(str);

		tsize = min(tsize, GenSize);

		memcpy(this->dataInternal, str, tsize * sizeof(FChar));
		this->dataInternal[tsize] = '\0';
	}



	size_t GetStrLen() const
	{
		return wcslen(this->dataInternal);
	}

	FChar* ToWCString() const
	{
		return this->dataInternal;
	}
};

template<size_t GenSize>
bool operator==(const FStaticString<GenSize>& str, const FStaticString<GenSize>& str2)
{
	return str.GetStrLen() == str2.GetStrLen() && !memcmp(&str[0], &str2[0], str.GetStrLen() * sizeof(FChar));
}
template<size_t GenSize>
bool operator!=(const FStaticString<GenSize>& str, const FStaticString<GenSize>& str2)
{
	return !(str == str2);
}



EXPORT(struct, FString) : public FArray<FChar>
{


public:
	FString();
	FString(CstrLiteral);
	FString(WCstrLiteral);
	FString(const FString& f);
	FString(FString&& f) noexcept;


	FString(int32_t);
	FString(int64_t);
	FString(uint32_t);
	FString(uint64_t);
	FString(float);
	FString(double);
	FString(char);
	FString(wchar_t);
	FString(bool);


	FArray<FChar> ToCharArray();

	FChar* ToWCString() const;



	int FindFirst(const FString& fstring, const FString& what);
	FArray<int> FindAll(const FString& fstring, const FString& what);

	void Replace(const FString& what, const FString& withWhat);



	FString& operator=(const FString&);
	FString& operator=(FString&&);

	FString& operator=(CstrLiteral);
	FString& operator=(WCstrLiteral);


	FString& operator+=(const FString&);
	FString& operator+=(CstrLiteral);
	FString& operator+=(WCstrLiteral);

	FString& operator+=(char);
	FString& operator+=(wchar_t);


	template<typename...TArgs>
	static FString Format(const FString& Format, const TArgs&... args);
};

EXPORT(struct, FStringFormatArg)
{
	enum EType { Int, UInt, Double, String, StringLiteral };

	EType Type;

	union
	{
		int64_t IntValue;
		uint64_t UIntValue;
		double DoubleValue;
		const FChar* StringLiteralValue;
	};

	FString StringValue;

	FStringFormatArg(const int32_t Value) : Type(Int), IntValue(Value) {}
	FStringFormatArg(const uint32_t Value) : Type(UInt), UIntValue(Value) {}
	FStringFormatArg(const int64_t Value) : Type(Int), IntValue(Value) {}
	FStringFormatArg(const uint64_t Value) : Type(UInt), UIntValue(Value) {}
	FStringFormatArg(const float Value) : Type(Double), DoubleValue(Value) {}
	FStringFormatArg(const double Value) : Type(Double), DoubleValue(Value) {}
	FStringFormatArg(FString Value) : Type(String), StringValue(Value) {}
	FStringFormatArg(const FChar* Value) : Type(StringLiteral), StringLiteralValue(Value) {}
	FStringFormatArg(const FStringFormatArg& RHS)
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

	FStringFormatArg();
};


template<typename...TArgs>
FString FString::Format(const FString& Format, const TArgs&... args)
{

	FArray<FStringFormatArg> formatargs = { FStringFormatArg(args) ... };

	FString result = "";

	int formatReq = 0;

	for (size_t i = 0; i < Format.size; i++)
	{
		if (Format[i] != L'%')
		{
			result += Format[i];
		}
		else
		{

			int formatIndex = (Format[i++ + 1] - L'0');

			if (Format[i] == L'%')
			{
				result += L'%';
			}

			if (formatIndex >= 0 && formatIndex <= 9)
			{

				FStringFormatArg::EType debugt = formatargs[formatIndex].Type;

				if (debugt == FStringFormatArg::Double)
				{
					result += FString(formatargs[formatIndex].DoubleValue);

				}
				if (debugt == FStringFormatArg::Int)
				{
					result += FString(formatargs[formatIndex].IntValue);
				}
				if (debugt == FStringFormatArg::UInt)
				{
					result += FString(formatargs[formatIndex].UIntValue);

				}
				if (debugt == FStringFormatArg::StringLiteral)
				{
					result += FString(formatargs[formatIndex].StringLiteralValue);
				}
				if (debugt == FStringFormatArg::String)
				{
					result += formatargs[formatIndex].StringValue;
				}
			}
		}
	}

	return result;
}



bool operator==(const FString& str, const FString&);
bool operator!=(const FString& str, const FString&);


const FString& operator+(const FString& str, const FString&);
FString operator+(const FString& str, CstrLiteral);
FString operator+(const FString& str, WCstrLiteral);

