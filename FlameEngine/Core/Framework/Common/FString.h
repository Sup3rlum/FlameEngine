#pragma once


#include "Core/Common/CoreBase.h"
#include "FArray.h"
#include <cstring>

EXPORT(class, FString) : public FArray<WCHAR>
{

	typedef const char* CstrLiteral;
	typedef const wchar_t* WCstrLiteral;
	typedef char* Cstr;
	typedef wchar_t* WCstr;

public:
	FString();
	FString(CstrLiteral);
	FString(WCstrLiteral);
	FString(const FString& f);

	WCHAR* ToCharPtr();
	FArray<WCHAR> ToCharArray();

	FString& operator=(const FString&);

	FString& operator=(CstrLiteral);
	FString& operator=(WCstrLiteral);

	FString& operator+(const FString&);
	FString& operator+(CstrLiteral);
	FString& operator+(WCstrLiteral);


	FString& operator+(float);
	FString& operator+(double);

	FString& operator+(int32_t);
	FString& operator+(int64_t);
};

