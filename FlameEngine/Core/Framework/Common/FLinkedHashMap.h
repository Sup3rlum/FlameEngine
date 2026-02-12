#pragma once

#include "FString.h"
#include "FHash.h"
#include "FKeyVal.h"
#include "FLinkedList.h"

struct FLinkedHashMapAllocator
{
	enum
	{
		RehashAllocIndex = 2,
	};
};



template<typename _TKey, typename TValue, typename THasher>
struct FLinkedHashMap;


template<typename _TKey, typename TValue, typename THasher>
struct FLinkedHashMapIterator
{
private:
	typedef FRemoveConst<_TKey> TKey;

	typedef FKeyVal<TKey, TValue> KvType;
	typedef FLinkedListNode<KvType*> _KvType;

	typedef FLinkedHashMap<TKey, TValue, THasher> _MapType;


	FLinkedHashMapIterator& End()
	{
		currentElement = NULL;
		return *this;
	}

public:

	FLinkedHashMapIterator(const _MapType* map) : FLinkedHashMapIterator(map, NULL)
	{}


	FLinkedHashMapIterator(const _MapType* map, _KvType* element) :
		currentElement(element),
		map(map)
	{}

	FLinkedHashMapIterator(const FLinkedHashMapIterator& it) : FLinkedHashMapIterator(it.map, it.currentElement)
	{}

	FLinkedHashMapIterator& operator++(int)
	{
		FLinkedHashMapIterator pre = *this;
		++(*this);
		return pre;
	}
	FLinkedHashMapIterator& operator++()
	{
		if (currentElement->Next)
		{
			currentElement = currentElement->Next;
			return *this;
		}

		return End();

	}
	FLinkedHashMapIterator& operator=(const FLinkedHashMapIterator& other)
	{
		map = other.map;
		currentElement = other.currentElement;

		return *this;
	}

	KvType* operator->()
	{
		return &currentElement->Value;
	}
	KvType& operator*()
	{
		return *(*currentElement).Value;
	}
	friend bool operator==(const FLinkedHashMapIterator& it1, const FLinkedHashMapIterator& it2)
	{
		return it1.currentElement == it2.currentElement;
	}

	friend bool operator!=(const FLinkedHashMapIterator& it1, const FLinkedHashMapIterator& it2)
	{
		return !(it1 == it2);
	}

	operator FLinkedHashMapIterator<const TKey, TValue, THasher>() const
	{
		FLinkedHashMapIterator it(map);
		it.currentElement = currentElement;
		return it;
	}


private:
	const _MapType* map;
	_KvType* currentElement;
};

template<typename TKey, typename TValue, typename THasher = FHash<TKey>>
class FLinkedHashMap
{
	typedef FKeyVal<TKey, TValue> KvType;
	typedef FKeyVal<const TKey, TValue> ConstKvType;

	typedef FLinkedListNode<KvType> _BucketType;
	typedef FLinkedListNode<ConstKvType> _ConstBucketType;

	typedef _BucketType* _BucketTypePtr;
	typedef const _BucketType* _ConstBucketTypePtr;
	typedef FArray<_BucketType*> _BucketArray;


	FLinkedList<KvType*> InsertionList;

public:

	typedef FLinkedHashMapIterator<TKey, TValue, THasher> Iterator;
	typedef FLinkedHashMapIterator<const TKey, TValue, THasher> ConstIterator;



	size_t TableSize;
	size_t ElementCount;
	_BucketArray bucketList;


	FLinkedHashMap(size_t initTableSize) :
		ElementCount(0),
		TableSize(initTableSize)
	{
		bucketList.Resize(TableSize);
		ZeroBuckets();

	}
	FLinkedHashMap() : FLinkedHashMap(20)
	{
	}

	FLinkedHashMap(const FLinkedHashMap& other) : FLinkedHashMap(other.TableSize)
	{
		for (const auto& kv : other)
		{
			Set(kv.Key, kv.Value);
		}
	}

	FLinkedHashMap(FLinkedHashMap&& other) noexcept :
		bucketList(MoveRef(other.bucketList)),
		TableSize(other.TableSize),
		ElementCount(ElementCount)
	{
	}
	FLinkedHashMap& operator=(const FLinkedHashMap& other)
	{
		Clear();

		for (const auto& kv : other)
		{
			Set(kv.Key, kv.Value);
		}

		return *this;
	}

	FLinkedHashMap& operator=(FLinkedHashMap&& other) noexcept
	{
		bucketList = MoveRef(other.bucketList);
		TableSize = other.TableSize;
		ElementCount = other.ElementCount;

		return *this;
	}

	const _ConstBucketTypePtr& FindBucket(const TKey& key) const
	{
		size_t hash = THasher{}(key);
		size_t bucketIndex = hash % TableSize;

		return bucketList[bucketIndex];
	}

	_BucketTypePtr& FindBucket(const TKey& key)
	{
		size_t hash = THasher{}(key);
		size_t bucketIndex = hash % TableSize;

		return bucketList[bucketIndex];
	}


	FLinkedHashMap& Set(const TKey& key, const TValue& value)
	{
		return Set(KvType(key, value));
	}
	FLinkedHashMap& Set(const KvType& kv)
	{
		EnsureCapacity();

		_BucketType** elemPtr = &FindBucket(kv.Key);
		_BucketType** prev = elemPtr;

		while (*elemPtr)
		{
			if ((*elemPtr)->Value.Key == kv.Key)
			{
				(*elemPtr)->Value.Value = kv.Value;
				return *this;
			}

			prev = elemPtr;
			elemPtr = &(*elemPtr)->Next;
		}

		*elemPtr = new _BucketType(kv);

		if (prev != elemPtr)
		{
			(*prev)->Next = *elemPtr;
		}

		ElementCount++;
		InsertionList.InsertTop(&((*elemPtr)->Value));

		return *this;
	}


	TValue& Get(const TKey& key)
	{
		_BucketType* elemPtr = FindBucket(key);

		while (elemPtr)
		{
			if (elemPtr->Value.Key == key)
			{
				return elemPtr->Value.Value;
			}

			elemPtr = elemPtr->Next;
		}

		Set(key, TValue());

		return Get(key);
	}

	const TValue& Get(const TKey& key) const
	{
		auto elemPtr = FindBucket(key);

		while (elemPtr)
		{
			if (elemPtr->Value.Key == key)
			{
				return elemPtr->Value.Value;
			}

			elemPtr = elemPtr->Next;
		}

		return TValue();
	}


	void Delete(const TKey& key)
	{
		_BucketTypePtr* elemRef = &FindBucket(key);

		while ((*elemRef))
		{
			if ((*elemRef)->Value.Key == key)
			{
				auto elemNext = (*elemRef)->Next;
				auto elemPtr = (*elemRef);
				elemPtr->Next = NULL;
				delete elemPtr;

				*elemRef = elemNext;

				ElementCount--;

				return;
			}
			elemRef = &(*elemRef)->Next;
		}
	}


	TValue& operator[](const TKey& key)
	{
		return Get(key);
	}

	const TValue& operator[](const TKey& key) const
	{
		return Get(key);
	}


	bool Contains(const TKey& key) const
	{
		const _BucketType* elemPtr = FindBucket(key);

		while (elemPtr)
		{
			if (elemPtr->Value.Key == key)
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
			_BucketType* elemPtr = bucketList[i];

			while (elemPtr)
			{
				keyArr.Add(elemPtr->Value.Key);

				elemPtr = elemPtr->Next;
			}
		}

		return keyArr;
	}

	FArray<KvType> GetAllKeyValPairs()
	{
		FArray<KvType> keyValArr;

		for (int i = 0; i < bucketList.Length(); i++)
		{

			_BucketType* elemPtr = bucketList[i];

			while (elemPtr)
			{
				keyValArr.Add(elemPtr->Value);

				elemPtr = elemPtr->Next;
			}
		}

		return keyValArr;
	}

	FLinkedHashMap& Clear()
	{
		for (auto bucketPtr : bucketList)
		{
			if (bucketPtr)
			{
				delete bucketPtr;
			}

			bucketPtr = NULL;
		}

		ZeroBuckets();
		ElementCount = 0;

		return *this;
	}


	Iterator Begin()
	{
		return begin();
	}


	Iterator End()
	{
		return end();
	}


	Iterator begin()
	{
		return Iterator(this, InsertionList.Root);
	}
	Iterator end()
	{
		return Iterator(this, NULL);
	}


	/* Const Iterator */

	ConstIterator begin() const
	{
		return ConstIterator(this, InsertionList.Root);
	}
	ConstIterator end() const
	{
		return ConstIterator(this, NULL);
	}


	~FLinkedHashMap()
	{
		for (auto bucketPtr : bucketList)
		{
			if (bucketPtr)
			{
				delete bucketPtr;
			}
		}
	}

private:

	FArray<KvType*> GetNonEmptyBuckets()
	{
		FArray<KvType*> arr;

		auto node = InsertionList.Root;
		while (node)
		{
			arr.Add(node->Value);
			node = node->Next;
		}

		return arr;
	}

	void ZeroBuckets()
	{
		Memory::Zero(bucketList.Begin(), bucketList.ByteSize());
	}

	void EnsureCapacity()
	{
		if (TableSize <= 0)
		{
			TableSize = FLinkedHashMapAllocator::RehashAllocIndex;
			bucketList.Resize(TableSize);
			ZeroBuckets();
		}

		float check = (float)ElementCount / (float)TableSize;
		if (check > 0.7f)
		{
			Rehash();
		}
	}

	void Rehash()
	{
		auto collectEntries = GetNonEmptyBuckets();

		ElementCount = 0;
		TableSize *= FLinkedHashMapAllocator::RehashAllocIndex;
		bucketList.Resize(TableSize);
		ZeroBuckets();
		InsertionList.Clear();

		for (auto& kv_pair : collectEntries)
		{
			Set(*kv_pair);
		}
	}
};
