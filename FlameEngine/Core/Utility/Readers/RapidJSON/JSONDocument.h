#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Framework/IO/FileStream.h"

/*
#include "rapidjson/document.h"

EXPORT(class, JSONDocument)
{
public:
	JSONDocument(FAnsiString jsonString)
	{
		d.Parse(jsonString.ToPlatformString());
	}
	JSONDocument(IOFileStream fileStream) : JSONDocument(fileStream.ReadAnsiFile())
	{

		
	}



	rapidjson::Value& operator[](FAnsiString name)
	{
		return d[name.ToPlatformString()];
	}


	const rapidjson::Value& operator[](FAnsiString name) const
	{
		return d[name.ToPlatformString()];
	}

private:
	rapidjson::Document d;

};*/