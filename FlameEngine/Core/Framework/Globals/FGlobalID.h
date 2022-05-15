#pragma once



/*
* 
* 
*	Defines a way to create unique IDs within the scope of the engine runtime instance
*	that are comparable and searchable
* 
* 
*/


#include "Core/Common/CoreCommon.h"
#include "Core/Engine/Common/FTime.h"
#include <windows.h>



EXPORT(class, FGlobalID)
{
private:

	struct FGUIDType
	{
		union
		{
			struct
			{
				GUID win32Guid;
			};
			struct
			{
				uint64 high, low;
			};
			struct
			{
				uint32 data1;
				uint16_t data2;
				uint16_t data3;
				byte data4[8];
			};
		};
	};




	FString nameInternal;
	FGUIDType guidInternal;


public:
	FGlobalID(FString name) : nameInternal(name), guidInternal(GenerateGUID())
	{

	}

	const FString& GetName() const
	{
		return nameInternal;
	}
	FString GetFullString()
	{
		FString str(nameInternal);

		WCstrLiteral guidStrLit = new FChar[40];
		//StringFromGUID2(guidInternal.win32Guid, guidStrLit, 40);
		
		return str;
	}

private:
	FGUIDType GenerateGUID()
	{
		FGUIDType t;

		CoCreateGuid(&t.win32Guid);

		return t;
	}
};
