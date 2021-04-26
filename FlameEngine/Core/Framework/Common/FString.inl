


/*
*  Compares two string by value.
*/

template<typename TBaseChar>
FORCEINLINE bool operator==(const TString<TBaseChar>& first, const TString<TBaseChar>& second)
{
	return (typename TString<TBaseChar>::BaseType)first == (typename TString<TBaseChar>::BaseType)second;
}

/*
*  Compares two strings by value.
*/
template<typename TBaseChar>
FORCEINLINE bool operator!=(const TString<TBaseChar>& first, const TString<TBaseChar>& second)
{
	return !(first == second);
}

template<typename TBaseChar>
FORCEINLINE TString<TBaseChar>& operator+(const TString<TBaseChar>& first, const TString<TBaseChar>& second)
{
	TString<TBaseChar> Result;
	Result.AddArray(first);
	Result.AddArray(second);

	return Result;
}



template<typename TBasicChar, size_t GenSize>
FORCEINLINE bool operator==(const TStaticString<TBasicChar, GenSize>& str, const TStaticString<TBasicChar, GenSize>& str2)
{
	return str.GetStrLen() == str2.GetStrLen() && !memcmp(&str[0], &str2[0], str.GetStrLen() * sizeof(TBasicChar));
}
template<typename TBasicChar, size_t GenSize>
FORCEINLINE bool operator!=(const TStaticString<TBasicChar, GenSize>& str, const TStaticString<TBasicChar, GenSize>& str2)
{
	return !(str == str2);
}
