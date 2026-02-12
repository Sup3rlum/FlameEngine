#pragma once

template<typename TKey, typename TValue>
struct FKeyVal
{
public:
	TKey Key;
	TValue Value;

	FKeyVal(const TKey& key, const TValue& value) : Key(key), Value(value)
	{}

	FKeyVal(const FKeyVal& kv) :
		Key(kv.Key),
		Value(kv.Value)
	{}


	FKeyVal& operator=(const FKeyVal& other)
	{
		Key = other.Key;
		Value = other.Value;

		return *this;
	}

	operator FKeyVal<const TKey, TValue>()
	{
		return FKeyVal<const TKey, TValue>(Key, Value);
	}

	virtual ~FKeyVal()
	{}
};
