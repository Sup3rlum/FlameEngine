#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Utility/Readers/RapidJSON/JSONDocument.h"


struct ConfigValue
{
	union
	{
		FString8* string;
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
	ConfigValue(const FString8& str) : string(new FString8(str)), _type(Type::String) {}

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
	FString8 GetString(const FString8& name);
	int			GetInt(const FString8& name);
	float		GetFloat(const FString8& name);
	bool		GetBool(const FString8& name);
	
	void Load(const FString& filepath);

private:
	FHashMap<FString8, ConfigValue> Data;
};