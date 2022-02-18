


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


/*
* String formatting
*/

template<typename TChar>
template<typename...TArgs>
TString<TChar> TString<TChar>::Format(const TString<TChar>& format, const TArgs& ... args)
{
	typedef TString<TChar> TStringType;

	FArray<TStringType> formatargs = { TStringType(args) ... };

	TStringType result;
	result.Reserve(format.Length()); // Reserve atleast the amount of characters in the format (best case, no reallactions for output string)

	for (size_t i = 0; i < format.Length(); i++)
	{
		TStringType fmtArgString;

		// Add regular characters to output
		if (format[i] != FStringFormatterSpecifier<TChar>::Percent())
		{
			fmtArgString = format[i];
		}
		// If Percent, get argument index and add formatted arg to output
		else
		{
			if (i == format.Length() - 1) // if this is the last symbol of the format, we are done 
			{
				return result;
			}

			int formatIndex = (format[++i] - FStringFormatterSpecifier<TChar>::Zero() /* "0" */);

			// Check if this was an escape character
			if (format[i] == FStringFormatterSpecifier<TChar>::Percent())
			{
				fmtArgString = FStringFormatterSpecifier<TChar>::Percent() /* % */;
			}
			else
			{
				if (formatIndex >= 0 && formatIndex <= 9)
				{
					fmtArgString = formatargs[formatIndex];
				}
			}
		}

		result += fmtArgString;
	}

	return result;
}
