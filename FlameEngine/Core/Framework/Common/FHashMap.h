#pragma once

#include "FString.h"
#include "FHash.h"

template<typename TKey, typename TValue>
struct FKeyVal
{
public:
	TKey Key;
	TValue Value;

	FKeyVal(const TKey& key, const TValue& value) : Key(key), Value(value)
	{}
};

struct FHashMapAllocator
{
	enum
	{
		RehashAllocIndex = 2,
	};
};





template<typename TKey, typename TValue, typename THasher = FHash<TKey>>
class FHashMap
{
	struct FMapBucketElement : public FKeyVal<TKey, TValue>
	{
		FMapBucketElement(TKey key, TValue value) : FKeyVal<TKey, TValue>(key, value)
		{}

		FMapBucketElement* Next = NULL;
	};

public:

	struct Iterator
	{
	public:
		Iterator() : Iterator(NULL)
		{}
		Iterator(FHashMap* mapPtr) : Iterator(mapPtr, NULL, 0)
		{}
		Iterator(FHashMap* mapPtr, FMapBucketElement* element, size_t bucketIndex) :
			currentElement(element),
			bucketIndex(bucketIndex),
			mapPtr(mapPtr)
		{}
		Iterator(const Iterator& it) : Iterator(it.mapPtr, it.currentElement, it.bucketIndex)
		{}

		const Iterator& operator++(int)
		{
			if (currentElement == NULL)
			{
				if (bucketIndex >= mapPtr->TableSize - 1)
				{
					bucketIndex = mapPtr->TableSize;
					currentElement = NULL;
					return *this;
				}
				else
				{
					currentElement = mapPtr->bucketList[++bucketIndex];
				}
			}
			else
			{
				currentElement = currentElement->Next;
			}

			if (currentElement == NULL)
			{
				return (*this)++;
			}

			return *this;
		}
		const Iterator& operator=(const Iterator& other)
		{
			currentElement = other.currentElement;
			mapPtr = other.mapPtr;
			bucketIndex = other.bucketIndex;

			return *this;
		}

		FORCEINLINE FKeyVal<TKey, TValue>* operator->()
		{
			return currentElement;
		}
		FORCEINLINE FKeyVal<TKey, TValue>& operator*()
		{
			return *currentElement;
		}
		friend bool operator==(const Iterator& it1, const Iterator& it2)
		{
			return it1.currentElement == it2.currentElement;
		}

		friend bool operator!=(const Iterator& it1, const Iterator& it2)
		{
			return !(it1 == it2);
		}


		friend class FHashMap;


	private:
		FHashMap* mapPtr;
		FMapBucketElement* currentElement;
		size_t bucketIndex;
	};





	size_t TableSize;
	size_t ElementCount;
	FArray<FMapBucketElement*> bucketList;


	FHashMap(size_t initTableSize) :
		ElementCount(0),
		TableSize(initTableSize)
	{
		bucketList.Resize(TableSize);
		Memory::Zero(bucketList.Begin(), bucketList.ByteSize());

	}
	FHashMap() : FHashMap(20)
	{

	}


	FHashMap& Set(const TKey& key, const TValue& value)
	{
		float check = (float)ElementCount / (float)TableSize;

		if (check > 0.7f)
		{
			Rehash();
		}

		size_t hash = THasher{}(key);
		size_t bucketIndex = hash % TableSize;

		FMapBucketElement** elemPtr = &bucketList[bucketIndex];
		FMapBucketElement** prev = elemPtr;

		while (*elemPtr)
		{
			if ((*elemPtr)->Key == key)
			{
				(*elemPtr)->Value = value;
				return *this;
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

		return *this;
	}


	TValue& Get(const TKey& key)
	{
		size_t hash = THasher{}(key);
		size_t bucketIndex = hash % TableSize;

		FMapBucketElement* elemPtr = bucketList[bucketIndex];

		while (elemPtr)
		{
			if (elemPtr->Key == key)
			{
				return elemPtr->Value;
			}
			
			elemPtr = elemPtr->Next;
		}
	}

	const TValue& Get(const TKey& key) const
	{
		size_t hash = THasher{}(key);
		size_t bucketIndex = hash % TableSize;

		FMapBucketElement* elemPtr = bucketList[bucketIndex];

		while (elemPtr)
		{
			if (elemPtr->Key == key)
			{
				return elemPtr->Value;
			}

			elemPtr = elemPtr->Next;
		}
	}


	bool Contains(const TKey& key)
	{
		size_t hash = THasher{}(key);
		size_t bucketIndex = hash % TableSize;

		FMapBucketElement* elemPtr = bucketList[bucketIndex];

		while (elemPtr)
		{
			if (elemPtr->Key == key)
			{
				return true;
			}

			elemPtr = elemPtr->Next;
		}

		return false;
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

	FHashMap& Clear()
	{



		return *this;
	}


	Iterator Begin()
	{
		Iterator it = End();

		for (int bucketIndex = 0; bucketIndex < TableSize; bucketIndex++)
		{
			FMapBucketElement* elemPtr = bucketList[bucketIndex];

			if (elemPtr != NULL)
			{
				it = Iterator(this, elemPtr, bucketIndex);
				break;
			}
		}
		return it;
	}


	Iterator End()
	{
		return Iterator(this, 0, TableSize);
	}


	const Iterator begin() const
	{
		Iterator it = End();

		for (int bucketIndex = 0; bucketIndex < TableSize; bucketIndex++)
		{
			FMapBucketElement* elemPtr = bucketList[bucketIndex];

			if (elemPtr != NULL)
			{
				it = Iterator(this, elemPtr, bucketIndex);
				break;
			}
		}
		return it;
	}
	const Iterator end() const
	{
		return Iterator(this, 0, TableSize);
	}




	FHashMap(const FHashMap& other) : FHashMap(other.TableSize)
	{
		//Iterator b = other.begin();
		//Iterator e = other.end();


		/*
		for (Iterator it = b; it != e; it++)
		{
			Set(it->Key, it->Value);
		}*/
	}


private:

	void Rehash()
	{
		FArray<FKeyVal<TKey, TValue>> collectEntries = GetAllKeyValPairs();

		ElementCount = 0;
		TableSize *= FHashMapAllocator::RehashAllocIndex;
		bucketList.Resize(TableSize);
		Memory::Zero(bucketList.Begin(), bucketList.ByteSize());


		for (int i = 0; i < collectEntries.Length(); i++)
		{
			Set(collectEntries[i].Key, collectEntries[i].Value);
		}
	}
};
