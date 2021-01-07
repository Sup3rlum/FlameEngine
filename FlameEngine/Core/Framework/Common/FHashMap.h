#pragma once

#include "FString.h"
#include "FHash.h"

template<typename TKey, typename TValue>
EXPORT(struct, FKeyVal)
{
public:
	TKey Key;
	TValue Value;

	FKeyVal(TKey key, TValue value) : Key(key), Value(value)
	{}
};


template<typename TKey, typename TValue>
EXPORT(class, FHashMap)
{
public:
	struct FMapBucketElement : public FKeyVal<TKey, TValue>
	{
	public:

		FMapBucketElement(TKey key, TValue value) : FKeyVal<TKey, TValue>(key, value)
		{}

		FMapBucketElement* Next = NULL;
	};


	size_t TableSize;
	size_t ElementCount;
	FArray<FMapBucketElement*> bucketList;


	FHashMap()
	{
		ElementCount = 0;
		TableSize = 10;
		bucketList.Resize(TableSize);
		Memory::Zero(bucketList.Begin(), bucketList.ByteSize());
	}

	void Set(TKey key, TValue value)
	{

		float check = (float)ElementCount / (float)TableSize;


		cout << "Check: " << check << endl;

		if (check > 0.7f)
		{
			Rehash();
		}

		size_t hash = FHash<TKey>{}(key);

		size_t bucketIndex = hash % TableSize;

		FMapBucketElement** elemPtr = &bucketList[bucketIndex];
		FMapBucketElement** prev = elemPtr;

		while (*elemPtr)
		{
			if ((*elemPtr)->Key == key)
			{
				(*elemPtr)->Value = value;
				return;
			}

			prev = elemPtr;
			elemPtr = &(*elemPtr)->Next;
		}

		*elemPtr = new FMapBucketElement(key, value);

		if (prev != elemPtr)
		{
			(*prev)->Next = *elemPtr;
		}

		ElementCount++;
	}
	TValue Get(TKey key)
	{
		size_t hash = FHash<TKey>{}(key);


		size_t bucketIndex = hash % TableSize;

		FMapBucketElement** elemPtr = &bucketList[bucketIndex];

		while (*elemPtr)
		{

			if ((*elemPtr)->Key == key)
			{
				return (*elemPtr)->Value;
			}
			elemPtr = &(*elemPtr)->Next;
		}
		return 0;
	}


	FArray<TKey> GetAllKeys()
	{
		FArray<TKey> keyArr;

		for (int i = 0; i < bucketList.Length(); i++)
		{

			FMapBucketElement* elemPtr = bucketList[i];

			while (elemPtr)
			{
				keyArr.Add(elemPtr->Key);

				elemPtr = elemPtr->Next;
			}
		}

		return keyArr;
	}

	FArray<FKeyVal<TKey, TValue>> GetAllKeyValPairs()
	{
		FArray<FKeyVal<TKey, TValue>> keyValArr;

		for (int i = 0; i < bucketList.Length(); i++)
		{

			FMapBucketElement* elemPtr = bucketList[i];

			while (elemPtr)
			{
				keyValArr.Add(*elemPtr);

				elemPtr = elemPtr->Next;
			}
		}

		return keyValArr;
	}

private:

	void Rehash()
	{
		FArray<FKeyVal<TKey, TValue>> collectEntries = GetAllKeyValPairs();

		ElementCount = 0;
		TableSize *= 2;
		bucketList.Resize(TableSize);
		Memory::Zero(bucketList.Begin(), bucketList.ByteSize());


		for (int i = 0; i < collectEntries.Length(); i++)
		{
			Set(collectEntries[i].Key, collectEntries[i].Value);
		}
	}
};



