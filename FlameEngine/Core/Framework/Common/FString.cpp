#include "FString.h"


FArray<WCHAR> FString::ToCharArray()
{
	return *(FArray<WCHAR>*)this;
}

TCHAR* FString::ToCharPtr()
{
	return ptrInternal;
}

FString::FString(CstrLiteral str)
{
	size_t tsize = strlen(str);

	size = tsize;
	capacity = tsize;

	ptrInternal = Memory::Create<WCHAR>(tsize);

	mbstowcs(ptrInternal, str, tsize);
}

FString::FString(WCstrLiteral str)
{
	size_t tsize = wcslen(str);

	size = tsize;
	capacity = tsize;
	ptrInternal = Memory::Create<WCHAR>(tsize);

	memcpy(ptrInternal, str, tsize);
}

FString::FString(const FString& f) : FArray<WCHAR>(f)
{


}


FString& FString::operator=(const FString& arr)
{
	FArray<WCHAR>::operator=(arr);
}
				
FString& FString::operator=(CstrLiteral str)
{
	size_t tsize = strlen(str);

	Resize(tsize);

	mbstowcs(ptrInternal, str, tsize);
}
FString& FString::operator=(WCstrLiteral str)
{
	size_t tsize = wcslen(str);

	Resize(tsize);

	memcpy(ptrInternal, str, tsize);
}
FString& FString::operator+(const FString&)
{

}
FString& FString::operator+(CstrLiteral)
{


}
FString& FString::operator+(WCstrLiteral)
{

}
FString& FString::operator+(float)
{

}
FString& FString::operator+(double)
{


}
FString& FString::operator+(int32_t)
{

}
FString& FString::operator+(int64_t)
{

}