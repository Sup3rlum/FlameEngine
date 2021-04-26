#include "FString.h"



/*
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

		//if (!wmemcmp(&fstring[i], what.Begin(), what.Length()))
		if (!memcmp(&fstring[i], what.Begin(), what.ByteSize()))
		{
			return i;
		}

	}
}*/

