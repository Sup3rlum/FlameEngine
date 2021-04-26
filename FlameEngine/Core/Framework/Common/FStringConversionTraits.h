#pragma once

#include "Core/Common/CoreBase.h"


typedef wchar_t FChar;
typedef const char* CstrLiteral;
typedef const wchar_t* WCstrLiteral;
typedef char* Cstr;
typedef wchar_t* WCstr;



template<typename TCharBase>
struct FBaseStringPlatformTypeConversionSpecifier;

template<>
struct FBaseStringPlatformTypeConversionSpecifier<char>
{
	template<typename TBasicType>
	static constexpr const char* GetSpecifier();

	template<> static constexpr const char* GetSpecifier<int32>()	{ return "%d"; }
	template<> static constexpr const char* GetSpecifier<uint32>()	{ return "%ud"; }
	template<> static constexpr const char* GetSpecifier<int64>()	{ return "%lld"; }
	template<> static constexpr const char* GetSpecifier<uint64>()	{ return "%ulld"; }
	template<> static constexpr const char* GetSpecifier<float>()	{ return "%f"; }
	template<> static constexpr const char* GetSpecifier<double>() { return "%lf"; }
	template<> static constexpr const char* GetSpecifier<char*>() { return "%s"; }
	template<> static constexpr const char* GetSpecifier<unsigned char*>()	{ return "%s"; }

};

template<>
struct FBaseStringPlatformTypeConversionSpecifier<FChar>
{
	template<typename TBasicType>
	static constexpr const FChar* GetSpecifier();

	template<> static constexpr const FChar* GetSpecifier<int32>() { return L"%d"; }
	template<> static constexpr const FChar* GetSpecifier<uint32>() { return L"%ud"; }
	template<> static constexpr const FChar* GetSpecifier<int64>() { return L"%lld"; }
	template<> static constexpr const FChar* GetSpecifier<uint64>() { return L"%ulld"; }
	template<> static constexpr const FChar* GetSpecifier<float>() { return L"%f"; }
	template<> static constexpr const FChar* GetSpecifier<double>() { return L"%lf"; }
	template<> static constexpr const FChar* GetSpecifier<FChar*>() { return L"%s"; }
};


template<typename TCharBase>
struct FStringFormatterSpecifier;

template<>
struct FStringFormatterSpecifier<FChar>
{
	static constexpr const FChar Percent() { return L'%'; }
	static constexpr const FChar Zero() { return L'0'; }
};
template<>
struct FStringFormatterSpecifier<char>
{
	static constexpr const char Percent() { return '%'; }
	static constexpr const char Zero() { return '0'; }
};