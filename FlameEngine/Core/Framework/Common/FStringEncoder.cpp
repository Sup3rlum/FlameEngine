#include "FString.h"


FString FStringEncoder::GetFromBytes(FArray<byte> bytes, EStringByteEncoding encoding)
{
	FString outputString;

	if (encoding == EStringByteEncoding::ANSI)
	{
		bytes.Add(0); // Null terminate string

		outputString = FString(reinterpret_cast<char*>(bytes.Begin()));
	}
	if (encoding == EStringByteEncoding::UTF8)
	{
		if (bytes.Length() % 2 == 1)
		{
			// TODO : invalid byte buffer
		}

		bytes.Add(0);
		bytes.Add(0);

		outputString = FString(reinterpret_cast<FChar*>(bytes.Begin()));

	}

	return outputString;
}