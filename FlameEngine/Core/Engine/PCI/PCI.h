#pragma once


#include "Core/Common/CoreCommon.h"

struct IProperty
{
private:
	enum Type
	{
		PropInt32,
		PropInt64,
		PropUint32,
		PropUint64,
		PropFloat,
		PropVector2,
		PropVector3,
		PropVector4,
		PropBool,
		PropString,
		PropEnum,
		PropNone
	} PropType;

	union
	{
		FVector4* Vector4Value;
		FVector3* Vector3Value;
		FVector2* Vector2Value;
		int64* Int64Value;
		int32* Int32Value;
		uint64* Uint64Value;
		uint32* Uint32Value;
		bool* BoolValue;
		float* FloatValue;
		FString* StringValue;
		void* EnumValue;
	};

	IProperty(const IProperty& other) : PropType(other.PropType), EnumValue(other.EnumValue) {}
	IProperty() : PropType(PropNone), EnumValue(0) {}

	IProperty& operator=(const IProperty& other)
	{
		PropType = other.PropType;
		EnumValue = other.EnumValue;

		return *this;
	}

	friend class FHashMap<FString, IProperty>;
	friend class FKeyVal<FString, IProperty>;
	friend class IProperties;

public:
	IProperty(FString* str) : StringValue(str),		PropType(PropString) {}
	IProperty(int64* value) : Int64Value(value),	PropType(PropInt64) {}
	IProperty(uint64* value) : Uint64Value(value),	PropType(PropUint64) {}
	IProperty(int32* value) : Int32Value(value),	PropType(PropInt32) {}
	IProperty(uint32* value) : Uint32Value(value),	PropType(PropUint32) {}
	IProperty(float* value) : FloatValue(value),	PropType(PropFloat) {}
	IProperty(bool* value) : BoolValue(value),		PropType(PropBool) {}

	IProperty(FVector2* value) : Vector2Value(value), PropType(PropVector2) {}
	IProperty(FVector3* value) : Vector3Value(value), PropType(PropVector3) {}
	IProperty(FVector4* value) : Vector4Value(value), PropType(PropVector4) {}
	
	template<typename TEnum>
	IProperty(TEnum* value) : EnumValue(value), PropType(PropEnum) {}

	bool GetBool() const
	{
		return *BoolValue;
	}

	const FString& GetString() const
	{
		return *StringValue;
	}

	int32 GetInt() const
	{
		return *Int32Value;
	}

	int64 GetInt64() const
	{
		return *Int64Value;
	}

	uint32 GetUInt() const
	{
		return *Uint32Value;
	}

	uint64 GetUInt64() const
	{
		return *Uint64Value;
	}

	float GetFloat() const
	{
		return *FloatValue;
	}

	const FVector4& GetVector4() const
	{
		return *Vector4Value;
	}
	const FVector3& GetVector3() const
	{
		return *Vector3Value;
	}
	const FVector2& GetVector2() const
	{
		return *Vector2Value;
	}

	template<typename TEnum>
	const TEnum& GetEnum() const 
	{
		return *(TEnum*)EnumValue;
	}


	void Set(const FString& str) { *StringValue = str; }
	void Set(int64 value) { *Int64Value = value; }
	void Set(int32 value) { *Int32Value = value; }
	void Set(uint64 value) { *Int64Value = value; }
	void Set(uint32 value) { *Uint32Value = value; }
	void Set(bool value) { *BoolValue = value; }
	void Set(float value) { *FloatValue = value; }

	void Set(FVector2 value) { *Vector2Value = value; }
	void Set(FVector3 value) { *Vector3Value = value; }
	void Set(FVector4 value) { *Vector4Value = value; }

	template<typename TEnum>
	void Set(TEnum value) { *(TEnum*)EnumValue = value; }
};

class IProperties
{
protected:
	FHashMap<FString, IProperty> Properties;

	void Init(const FString& name, const IProperty& prop)
	{
		this->Properties.Set(name, prop);
	}
public:
	IProperty& Property(const FString& name)
	{
		return Properties.Get(name);
	}

	const IProperty& Property(const FString& name) const
	{
		return Properties.Get(name);
	}

	virtual ~IProperties() {}
};

#define PropertyValue(type, propertyName, defaultValue)      \
														\
type propertyName = __Init##propertyName();				\
private:												\
type __Init##propertyName()							    \
{														\
this->Init(#propertyName, &propertyName);				\
return defaultValue;									\
}														\
														\
public:													\

#define PropertyBool(propertyName, defaultValue) PropertyValue(bool, propertyName, defaultValue)
#define PropertyInt(propertyName, defaultValue) PropertyValue(int32, propertyName, defaultValue)
#define PropertyInt64(propertyName, defaultValue) PropertyValue(int64, propertyName, defaultValue)
#define PropertyFloat(propertyName, defaultValue) PropertyValue(float, propertyName, defaultValue)
#define PropertyString(propertyName, defaultValue) PropertyValue(FString, propertyName, defaultValue)
#define PropertyEnum(enumType, propertyName, defaultValue) PropertyValue(enumType, propertyName, defaultValue)

#define PropertyVector2(propertyName, defaultValue) PropertyValue(FVector2, propertyName, defaultValue)
#define PropertyVector3(propertyName, defaultValue) PropertyValue(FVector3, propertyName, defaultValue)
#define PropertyVector4(propertyName, defaultValue) PropertyValue(FVector4, propertyName, defaultValue)


#define ThisProperty(a) this->a