

/*
*  Compares two arrays by value.
*  Requires that TType has == operator
*/
template<typename TType>
FORCEINLINE bool operator==(const FArray<TType>& first, const FArray<TType>& second)
{
	if (first.Length() != second.Length())
	{
		return false;
	}
	for (int i = 0; i < first.Length(); i++)
	{
		// This enforces only the requirement of an equality operator
		if (!(first[i] == second[i]))
			return false;
	}

	return true;
}


/*
*  Compares two arrays by value.
*  Requires that TType has == operator
*/
template<typename TType>
FORCEINLINE bool operator!=(const FArray<TType>& first, const FArray<TType>& second)
{
	return !operator==(first, second);
}
