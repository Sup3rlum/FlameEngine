#include "ConfigComponent.h"


#include <nlohmann/json.hpp>

FAnsiString ConfigModule::GetString(const FAnsiString& name)
{
	if (Data.Contains(name) && Data[name]._type == ConfigValue::Type::String)
	{
		return *Data[name].string;
	}
}

bool ConfigModule::GetBool(const FAnsiString& name)
{
	if (Data.Contains(name) && Data[name]._type == ConfigValue::Type::Bool)
	{
		return Data[name].boolVal;
	}
}

int ConfigModule::GetInt(const FAnsiString& name)
{
	if (Data.Contains(name) && Data[name]._type == ConfigValue::Type::Int)
	{
		return Data[name].intVal;
	}
}
float ConfigModule::GetFloat(const FAnsiString& name)
{
	if (Data.Contains(name) && Data[name]._type == ConfigValue::Type::Float)
	{
		return Data[name].floatVal;
	}
}


void ConfigModule::Load(const FString& str)
{
	IOFileStream file(str);
	nlohmann::json j;

	file._file >> j;

	for (auto& [key, value] : j.items())
	{
		if (value.is_string())
		{
			Data[key.c_str()] = value.get<std::string>().c_str();
		}
		if (value.is_boolean())
		{
			Data[key.c_str()] = value.get<bool>();
		}
		if (value.is_number_integer())
		{
			Data[key.c_str()] = value.get<int>();
		}
		if (value.is_number_float())
		{
			Data[key.c_str()] = value.get<float>();
		}
	}

}