#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Utility/Readers/RapidJSON/JSONDocument.h"




struct ConfigValue
{
	union
	{
		FAnsiString* string;
		float floatVal;
		int intVal;
		bool boolVal;
	};

private:
	enum class Type
	{
		String,
		Float,
		Int,
		Bool
	};
public:
	Type _type;

	ConfigValue() : intVal(0), _type(Type::Int) {}
	ConfigValue(int i) : intVal(i), _type(Type::Int) {}
	ConfigValue(float f) : floatVal(f), _type(Type::Float) {}
	ConfigValue(bool b) : boolVal(b), _type(Type::Bool) {}
	ConfigValue(const FAnsiString& str) : string(new FAnsiString(str)), _type(Type::String) {}

	~ConfigValue()
	{
		if (_type == Type::String)
			delete string;
	}

	friend class ConfigModule;
};

class ConfigModule
{
public:
	FAnsiString GetString(const FAnsiString& name);
	int			GetInt(const FAnsiString& name);
	float		GetFloat(const FAnsiString& name);
	bool		GetBool(const FAnsiString& name);
	
	void Load(const FString& filepath);

private:
	FHashMap<FAnsiString, ConfigValue> Data;
};