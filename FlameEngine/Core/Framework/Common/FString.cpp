#include "FString.h"


FString::FString() : FArray<FChar>()
{

}

FArray<FChar> FString::ToCharArray()
{
	return FArray<FChar>(ptrInternal, size);
}

FString::FString(CstrLiteral str) : FArray<FChar>()
{
	size_t tsize = strlen(str);

	size = tsize;
	capacity = tsize;

	ptrInternal = Memory::Create<FChar>(tsize);


	size_t outSize;

	mbstowcs_s(&outSize, ptrInternal, tsize + 1, str, tsize);
}

FString::FString(WCstrLiteral str) : FArray<FChar>()
{
	size_t tsize = wcslen(str);

	size = tsize;
	capacity = tsize;
	ptrInternal = Memory::Create<FChar>(tsize);

	memcpy(ptrInternal, str, tsize * sizeof(FChar));
}

FString::FString(const FString& f) : FArray<FChar>(f)
{


}
FString::FString(FString&& f) noexcept : FArray<FChar>(f)
{


}



FString& FString::operator=(const FString& arr)
{
	FArray<FChar>::operator=(arr);

	return *this;
}

FString& FString::operator=(CstrLiteral str)
{
	size_t tsize = strlen(str);

	Resize(tsize);

	size_t outSize;

	mbstowcs_s(&outSize, ptrInternal, tsize + 1, str, tsize);

	return *this;

}
FString& FString::operator=(WCstrLiteral str)
{
	size_t tsize = wcslen(str);

	Resize(tsize);

	memcpy(ptrInternal, str, tsize);

	return *this;

}


FChar* FString::ToWCString() const
{
	ptrInternal[size] = L'\0';
	return ptrInternal;
}


FString& FString::operator+=(const FString& other)
{

	this->AddArray(other);

	return *this;
}
FString& FString::operator+=(CstrLiteral cstrlit)
{
	this->AddArray(FString(cstrlit));

	return *this;
}
FString& FString::operator+=(WCstrLiteral wcstrlit)
{
	this->AddArray(FString(wcstrlit));

	return *this;
}

FString& FString::operator+=(char i)
{
	this->AddArray(FString(i));

	return *this;
}

FString& FString::operator+=(wchar_t i)
{
	this->AddArray(FString(i));


	return *this;
}

#pragma warning(disable:4996)


#define MAKE_CSTR_FROM_VALTYPE(valtype, param)	size_t length = _snwprintf(NULL, 0, valtype, param); \
												WCstr str = (FChar*)malloc((length + 1) * sizeof(FChar)); \
												_snwprintf(str, length + 1, valtype, param); \
												ptrInternal = str; \
												size = length; \
												capacity = length; \

FString::FString(double d)
{
	MAKE_CSTR_FROM_VALTYPE(L"%lf", d)
}
FString::FString(float f)
{
	MAKE_CSTR_FROM_VALTYPE(L"%f", f)
}


FString::FString(int32_t i)
{
	MAKE_CSTR_FROM_VALTYPE(L"%d", i)
}
FString::FString(int64_t i)
{
	MAKE_CSTR_FROM_VALTYPE(L"%ld", i)
}

FString::FString(uint32_t i)
{
	MAKE_CSTR_FROM_VALTYPE(L"%ud", i)
}
FString::FString(uint64_t i)
{
	MAKE_CSTR_FROM_VALTYPE(L"%uld", i)
}

FString::FString(bool b)
{
	if (b)
		FString("true");
	else
		FString("false");
}

FString::FString(char c)
{
	size = 1;
	capacity = 1;
	ptrInternal = Memory::Create<FChar>(1);
	ptrInternal[0] = (FChar)c;
}

FString::FString(wchar_t c)
{
	size = 1;
	capacity = 1;
	ptrInternal = Memory::Create<FChar>(1);
	ptrInternal[0] = c;
}


const FString& operator+(const FString& str, const FString& other)
{
	FString Result;
	Result.AddArray(str);
	Result.AddArray(other);

	return Result;
}
FString operator+(FString& str, CstrLiteral other)
{
	FString Result = str;
	Result += other;
	return Result;
}
FString operator+(FString& str, WCstrLiteral other)
{
	FString Result = str;
	Result += other;
	return Result;
}



bool operator==(const FString& first, const FString& second)
{
	if (first.Length() != second.Length())
	{
		return false;
	}
	for (int i = 0; i < first.Length(); i++)
	{
		if (first[i] != second[i])
			return false;
	}

	return true;
}

bool operator!=(const FString& first, const FString& second)
{
	return !operator==(first, second);
}



int FString::FindFirst(const FString& fstring, const FString& what)
{
	if (what.Length() > fstring.Length())
		return -1;


	for (int i = 0; i < fstring.Length(); i++)
	{
		// We have passed the point where there is not enough string left to match
		if (i + what.Length() > fstring.Length())
		{
			return -1;
		}

		if (!wmemcmp(&fstring[i], what.Begin(), what.Length()))
		{
			return i;
		}

	}
}
FArray<int> FString::FindAll(const FString& fstring, const FString& what)
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
		if (!wmemcmp(&fstring[i], what.Begin(), what.Length()))
		{
			occurances.Add(i);
		}

	}

	return occurances;
}
void FString::Replace(const FString& what, const FString& withWhat)
{

}

