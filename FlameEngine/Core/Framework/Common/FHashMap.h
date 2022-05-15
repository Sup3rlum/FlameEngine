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

struct FHashMapAllocator
{
	enum
	{
		RehashAllocIndex = 2,
	};
};


template<typename Ty>
struct FLinkedListNode
{
	typedef Ty _Type;

	FLinkedListNode(const Ty& value) :
		kv(value)
	{
	}
	~FLinkedListNode()
	{
		if (Next)
		{
			delete Next;
		}
	}

	Ty kv;
	FLinkedListNode* Next = NULL;
};

template<typename _TKey, typename TValue, typename THasher>
struct FHashMap;

template<typename _TKey, typename TValue, typename THasher>
struct FHashMapConstIterator
{
private:
	typedef FRemoveConst<_TKey> TKey;

	typedef FKeyVal<TKey, TValue> ConstKvType;
	typedef FLinkedListNode<ConstKvType> _ConstBucketType;

	typedef FHashMap<TKey, TValue, THasher> _MapType;

	FHashMapConstIterator& End()
	{
		bucketIndex = map->bucketList.Length();
		currentElement = NULL;
		return *this;
	}

public:

	FHashMapConstIterator(const _MapType* map) : FHashMapConstIterator(map, NULL, 0)
	{}

	FHashMapConstIterator(const _MapType* map, _ConstBucketType* element, size_t bucketIndex) :
		currentElement(element),
		bucketIndex(bucketIndex),
		map(map)
	{}

	FHashMapConstIterator(const FHashMapConstIterator& it) : FHashMapConstIterator(it.map, it.currentElement, it.bucketIndex)
	{}

	FHashMapConstIterator& operator++(int)
	{
		FHashMapConstIterator pre = *this;
		++(*this);
		return pre;
	}
	FHashMapConstIterator& operator++()
	{
		if (currentElement)
		{
			auto nextElem = currentElement->Next;

			while (!nextElem)
			{
				if (bucketIndex >= map->bucketList.Length() - 1)
					return End();

				nextElem = map->bucketList[++bucketIndex];
			}

			currentElement = nextElem;

			return *this;
		}

		return End();
	}
	FHashMapConstIterator& operator=(const FHashMapConstIterator& other)
	{
		map = other.map;
		bucketIndex = other.bucketIndex;
		currentElement = other.currentElement;

		return *this;
	}

	ConstKvType* operator->()
	{
		return &currentElement->kv;
	}
	ConstKvType& operator*()
	{
		return (*currentElement).kv;
	}
	friend bool operator==(const FHashMapConstIterator& it1, const FHashMapConstIterator& it2)
	{
		return it1.currentElement == it2.currentElement;
	}

	friend bool operator!=(const FHashMapConstIterator& it1, const FHashMapConstIterator& it2)
	{
		return !(it1 == it2);
	}

private:
	const _MapType* map;
	_ConstBucketType* currentElement;
	size_t bucketIndex;
};



template<typename _TKey, typename TValue, typename THasher>
struct FHashMapIterator
{
private:
	typedef FRemoveConst<_TKey> TKey;

	typedef FKeyVal<TKey, TValue> KvType;
	typedef FLinkedListNode<KvType> _BucketType;

	typedef FHashMap<TKey, TValue, THasher> _MapType;


	FHashMapIterator& End()
	{
		bucketIndex = map->bucketList.Length();
		currentElement = NULL;
		return *this;
	}

public:

	FHashMapIterator(_MapType* map) : FHashMapIterator(map, NULL, 0)
	{}


	FHashMapIterator(_MapType* map, _BucketType* element, size_t bucketIndex) :
		currentElement(element),
		bucketIndex(bucketIndex),
		map(map)
	{}

	FHashMapIterator(const FHashMapIterator& it) : FHashMapIterator(it.map, it.currentElement, it.bucketIndex)
	{}

	FHashMapIterator& operator++(int)
	{
		FHashMapIterator pre = *this;
		++(*this);
		return pre;
	}
	FHashMapIterator& operator++()
	{
		if (currentElement)
		{
			auto nextElem = currentElement->Next;

			while (!nextElem)
			{
				if (bucketIndex >= map->bucketList.Length() - 1)
					return End();

				nextElem = map->bucketList[++bucketIndex];
			}

			currentElement = nextElem;

			return *this;
		}

		return End();

	}
	FHashMapIterator& operator=(const FHashMapIterator& other)
	{
		map = other.map;
		bucketIndex = other.bucketIndex;
		currentElement = other.currentElement;

		return *this;
	}

	KvType* operator->()
	{
		return &currentElement->kv;
	}
	KvType& operator*()
	{
		return (*currentElement).kv;
	}
	friend bool operator==(const FHashMapIterator& it1, const FHashMapIterator& it2)
	{
		return it1.currentElement == it2.currentElement;
	}

	friend bool operator!=(const FHashMapIterator& it1, const FHashMapIterator& it2)
	{
		return !(it1 == it2);
	}

	operator FHashMapIterator<const TKey, TValue, THasher>() const
	{
		FHashMapIterator it(map);

		it.currentElement = currentElement;
		it.bucketIndex = bucketIndex;

		return it;
	}


private:
	_MapType* map;
	_BucketType* currentElement;
	size_t bucketIndex;
};

template<typename TKey, typename TValue, typename THasher = FHash<TKey>>
class FHashMap
{
	typedef FKeyVal<TKey, TValue> KvType;
	typedef FKeyVal<const TKey, TValue> ConstKvType;

	typedef FLinkedListNode<KvType> _BucketType;
	typedef FLinkedListNode<ConstKvType> _ConstBucketType;

	typedef _BucketType* _BucketTypePtr;
	typedef const _BucketType* _ConstBucketTypePtr;
	typedef FArray<_BucketType*> _BucketArray;


public:

	typedef FHashMapIterator<TKey, TValue, THasher> Iterator;
	typedef FHashMapConstIterator<const TKey, TValue, THasher> ConstIterator;



	size_t TableSize;
	size_t ElementCount;
	_BucketArray bucketList;


	FHashMap(size_t initTableSize) :
		ElementCount(0),
		TableSize(initTableSize)
	{
		bucketList.Resize(TableSize);
		ZeroBuckets();

	}
	FHashMap() : FHashMap(20)
	{
	}

	FHashMap(const FHashMap& other) : FHashMap(other.TableSize)
	{
		for (const auto& kv : other)
		{
			Set(kv.Key, kv.Value);
		}
	}

	FHashMap(FHashMap&& other) noexcept :
		bucketList(MoveRef(other.bucketList)),
		TableSize(other.TableSize),
		ElementCount(ElementCount)
	{
	}
	FHashMap& operator=(const FHashMap& other)
	{
		Clear();

		for (const auto& kv : other)
		{
			Set(kv.Key, kv.Value);
		}

		return *this;
	}

	FHashMap& operator=(FHashMap&& other) noexcept
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


	FHashMap& Set(const TKey& key, const TValue& value)
	{
		return Set(KvType(key, value));
	}
	FHashMap& Set(const KvType& kv)
	{
		EnsureCapacity();

		_BucketType** elemPtr = &FindBucket(kv.Key);
		_BucketType** prev = elemPtr;

		while (*elemPtr)
		{
			if ((*elemPtr)->kv.Key == kv.Key)
			{
				(*elemPtr)->kv.Value = kv.Value;
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

		return *this;
	}


	TValue& Get(const TKey& key)
	{
		_BucketType* elemPtr = FindBucket(key);

		while (elemPtr)
		{
			if (elemPtr->kv.Key == key)
			{
				return elemPtr->kv.Value;
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
			if (elemPtr->kv.Key == key)
			{
				return elemPtr->kv.Value;
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
			if ((*elemRef)->kv.Key == key)
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
			if (elemPtr->kv.Key == key)
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
				keyArr.Add(elemPtr->kv.Key);

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
				keyValArr.Add(elemPtr->kv);

				elemPtr = elemPtr->Next;
			}
		}

		return keyValArr;
	}

	FHashMap& Clear()
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
		Iterator it = end();

		for (int bucketIndex = 0; bucketIndex < TableSize; bucketIndex++)
		{
			_BucketType* elemPtr = bucketList[bucketIndex];

			if (elemPtr != NULL)
			{
				it = Iterator(this, elemPtr, bucketIndex);
				break;
			}
		}
		return it;
	}
	Iterator end()
	{
		return Iterator(this, 0, TableSize);
	}


	/* Const Iterator */

	ConstIterator begin() const
	{
		ConstIterator it = end();

		for (int bucketIndex = 0; bucketIndex < TableSize; bucketIndex++)
		{
			_BucketType* elemPtr = bucketList[bucketIndex];

			if (elemPtr != NULL)
			{
				it = ConstIterator(this, elemPtr, bucketIndex);
				break;
			}
		}
		return it;
	}
	ConstIterator end() const
	{
		return ConstIterator(this, 0, TableSize);
	}


	~FHashMap()
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

	_BucketArray GetNonEmptyBuckets()
	{
		_BucketArray out;
		for (auto bucket : bucketList)
		{
			if (bucket)
				out.Add(bucket);
		}

		return out;
	}

	void ZeroBuckets()
	{
		Memory::Zero(bucketList.Begin(), bucketList.ByteSize());
	}

	void EnsureCapacity()
	{
		if (TableSize <= 0)
		{
			TableSize = FHashMapAllocator::RehashAllocIndex;
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
		TableSize *= FHashMapAllocator::RehashAllocIndex;
		bucketList.Resize(TableSize);
		ZeroBuckets();

		for (auto& bucket : collectEntries)
		{
			auto b = bucket;
			while (b)
			{
				Set(b->kv);
				b = b->Next;
			}

			delete bucket;
		}
	}
};