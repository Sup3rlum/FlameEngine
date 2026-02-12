#pragma once


#include "Core/Common/CoreBase.h"
#include "Core/Common/TypeTraits.h"
#include "Core/Runtime/Common/Memory.h"
#include "Range.h"

template<typename GenType, size_t GenSize>
struct FStaticArray
{
public:
	typedef GenType* IteratorType;

	/*
	*	Default Constructor
	*/
	FStaticArray()
	{
	}

	/*
	*	From memory block Constructor
	*/

	FStaticArray(const GenType(&constArr)[GenSize])
	{
		Memory::CopyCounted<GenType>(dataInternal, constArr, GenSize);
	}
	FStaticArray(const GenType* constArr)
	{
		Memory::CopyCounted<GenType>(dataInternal, constArr, GenSize);
	}
	/*
	*	Copy Constructor
	*/
	FStaticArray(const FStaticArray<GenType, GenSize>&arr)
	{
		Memory::CopyCounted<GenType>(dataInternal, arr.dataInternal, GenSize);
	}
	/*
	*	C++ why you make me use std :(
	*/
	FStaticArray(const std::initializer_list<GenType>& list) : FStaticArray(static_cast<const GenType*>(list.begin())) 
	{
		assert(GenSize == list.size());
	}

	
	virtual ~FStaticArray()
	{
		//Memory::Free(dataInternal);
	}
	


	/* Sizing methods */
	constexpr size_t Length() const
	{
		return GenSize;
	}
	constexpr size_t AllocatorSize() const
	{
		return sizeof(GenType);
	}
	constexpr size_t ByteSize() const
	{
		return GenSize * sizeof(GenType);
	}

	/* State methods */
	bool IsEmpty()
	{
		return dataInternal == NULL || GenSize == 0;
	}


	/* Iteration */
	GenType* Begin()
	{
		return dataInternal;
	}
	const GenType* Begin() const
	{
		return dataInternal;
	}

	GenType* End()
	{
		return dataInternal + GenSize;
	}
	const GenType* End() const
	{
		return dataInternal + GenSize;
	}


	GenType& First()
	{
		return dataInternal[0];
	}
	const GenType& First() const
	{
		return dataInternal[0];
	}

	GenType& Last()
	{
		return dataInternal[GenSize - 1];
	}

	const GenType& Last() const
	{
		return dataInternal[GenSize - 1];
	}


	/* Operators */

	GenType& operator[](size_t index)
	{

		//static_assert(index < GenSize, "StaticArray Index out of range");

		return dataInternal[index];
	}

	const GenType& operator[](size_t index) const
	{

		//static_assert(index < GenSize, "StaticArray Index out of range");

		return dataInternal[index];
	}

	operator GenType* ()
	{
		return dataInternal;
	}

protected:
	GenType dataInternal[GenSize];
};


template<typename GenType>
struct FArray
{
public:

	typedef GenType* IteratorType;


	/*
	*	Default Constructor
	*/
	FArray() :
		size(0),
		capacity(0),
		ptrInternal(NULL)
	{
	}

	FArray(size_t initSize) :
		size(initSize),
		capacity(initSize)
	{
		ptrInternal = Memory::AllocCounted<GenType>(initSize);
		Memory::Zero(ptrInternal, size);
	}
	/*
	*	From memory block Constructor
	*/
	FArray(const GenType* constArr, size_t _Size) :
		size(_Size),
		capacity(_Size)
	{
		ptrInternal = Memory::AllocCounted<GenType>(_Size);
		Memory::Zero(ptrInternal, _Size * sizeof(GenType));

		Memory::CopyCounted<GenType>(ptrInternal, constArr, _Size);
	}
	/*
	*	From FStaticArray constructor
	*/
	template<size_t _Size>
	FArray(const FStaticArray<GenType, _Size>& statarr) : FArray(statarr.Begin(), _Size) {}

	/*
	*	Copy Constructor
	*/
	FArray(const FArray<GenType>& arr) : FArray(arr.ptrInternal, arr.size)
	{

	}
	/*
	*	Move Constructor
	*/
	FArray(FArray<GenType> && arr) :
		size(arr.size),
		capacity(arr.capacity),
		ptrInternal(arr.ptrInternal)
	{

		arr.size = 0;
		arr.capacity = 0;
		arr.ptrInternal = NULL;
	}
	/*
	*	C++ why you make me use std :(
	*/
	FArray(const std::initializer_list<GenType>& list) : FArray()
	{
		for (auto& elem : list)
		{
			this->Add(elem);
		}
	}

	/*
	*   Destructor
	*/

	
	virtual ~FArray()
	{
		Memory::Free(ptrInternal);
	}
	


	/* Sizing methods */
	size_t Length() const
	{
		return size;
	}
	size_t AllocatorSize() const
	{
		return sizeof(GenType);
	}
	size_t ByteSize() const
	{
		return size * sizeof(GenType);
	}

	/* State methods */
	bool IsEmpty()
	{
		return ptrInternal == NULL || size == 0 || capacity == 0;
	}
	/* Iteration */
	IteratorType Begin()
	{
		return ptrInternal;
	}
	const IteratorType Begin() const
	{
		return ptrInternal;
	}
	IteratorType End()
	{
		return ptrInternal + size;
	}
	const IteratorType End() const
	{
		return ptrInternal + size;
	}



	/* C++ range based for loop */

	IteratorType begin()
	{
		return ptrInternal;
	}
	const IteratorType begin() const
	{
		return ptrInternal;
	}
	IteratorType end()
	{
		return ptrInternal + size;
	}
	const IteratorType end() const
	{
		return ptrInternal + size;
	}


	/*----------------*/

	GenType& First()
	{
		return ptrInternal[0];
	}
	const GenType& First() const
	{
		return ptrInternal[0];
	}

	GenType& Last()
	{
		return ptrInternal[size - 1];
	}

	const GenType& Last() const
	{
		return ptrInternal[size - 1];
	}



	/* Operators */

	GenType& operator[](size_t index)
	{
		assert(index < size);

		return ptrInternal[index];
	}

	const GenType& operator[](size_t index) const
	{
		assert(index < size);

		return ptrInternal[index];
	}

	/* Range operators */


	FArray<GenType> operator[](const FRange& range)
	{
		assert(range.end() <= size);
		assert(range.begin() >= 0);

		size_t distance = range.end() - range.begin();
		assert(distance != 0);

		FArray<GenType> rArray;

		for (auto index : range)
		{
			rArray.Add(ptrInternal[index]);
		}

		return rArray;
	}

	/* ------------------------ */


	FArray<GenType>& operator=(const FArray<GenType> &arr)
	{
		//delete[] ptrInternal;

		size = arr.size;
		capacity = arr.capacity;
		ptrInternal = Memory::AllocCounted<GenType>(size);
		Memory::Zero(ptrInternal, size * sizeof(GenType));

		Memory::CopyCounted(ptrInternal, arr.ptrInternal, size);

		return *this;
	}


	/* Array Operations */
	FArray<GenType>& Add(const GenType & v)
	{
		if (size >= capacity)
			Reserve(max(capacity * 2, 1));

		ptrInternal[size++] = v;

		return *this;
	}


	FArray<GenType>& Add(GenType && v)
	{
		if (size >= capacity)
			Reserve(max(capacity * 2, 1));

		ptrInternal[size++] = MoveRef(v);

		return *this;
	}


	FArray<GenType>& AddArray(const FArray<GenType>&arr)
	{
		size_t oldSize = size;

		if (size + arr.size >= capacity)
			Resize(size + arr.size);
		else
			size += arr.size;

		Memory::CopyCounted(ptrInternal + oldSize, arr.ptrInternal, arr.size);

		return *this;
	}


	FArray<GenType>& Insert(const GenType & elem, size_t index)
	{

		if (index > size || ptrInternal == NULL)
			return *this;


		GenType* Newbuffer = Memory::AllocCounted<GenType>(size + 1);
		Memory::Zero(Newbuffer, (size + 1) * sizeof(GenType));

		Memory::CopyCounted(Newbuffer, ptrInternal, index);
		Memory::CopyCounted(&Newbuffer[index + 1], &ptrInternal[index], (size - index));

		Newbuffer[index] = elem;

		Memory::Free(ptrInternal);
		ptrInternal = Newbuffer;

		capacity = size + 1;
		size++;

		return *this;
	}
	FArray<GenType>& Insert(const FArray<GenType>&elems, size_t index)
	{

		if (index > size || ptrInternal == NULL || elems.Length() == 0)
			return *this;


		GenType* Newbuffer = Memory::AllocCounted<GenType>(size + elems.size);
		Memory::Zero(Newbuffer, (size + elems.size) * sizeof(GenType));

		Memory::CopyCounted(Newbuffer, ptrInternal, index);
		Memory::CopyCounted(&Newbuffer[index + elems.size], &ptrInternal[index], (size - index));

		Memory::CopyCounted(&Newbuffer[index], elems.Begin(), elems.size);

		Memory::Free(ptrInternal);
		ptrInternal = Newbuffer;

		capacity = size + elems.size;
		size += elems.size;

		return *this;
	}

	FArray<GenType>& Remove(size_t index)
	{
		Memory::CopyOverlap(&ptrInternal[index], &ptrInternal[index + 1], (size-- - index) * sizeof(GenType));

		return *this;
	}

	void Resize(size_t newsize)
	{
		Reserve(newsize);
		size = newsize;
	}


	void Reserve(size_t newcapacity)
	{
		if (ptrInternal == NULL)
		{
			size = 0;
			capacity = 0;
		}

		GenType* Newbuffer = Memory::AllocCounted<GenType>(newcapacity);
		Memory::Zero(Newbuffer, newcapacity * sizeof(GenType));
		uint32 l_Size = min(newcapacity, size);

		Memory::CopyCounted(Newbuffer, ptrInternal, l_Size);

		capacity = newcapacity;
		Memory::Free(ptrInternal);
		ptrInternal = Newbuffer;
	}

	template<typename TLambda>
	FArray<GenType> Where(TLambda&& lambda) const
	{
		FArray<GenType> result;

		for (const auto& elem : *this)
		{
			if (lambda(elem))
			{
				result.Add(elem);
			}
		}

		return result;
	}


	void Clear()
	{
		capacity = 0;
		size = 0;
		ptrInternal = NULL;
	}

protected:
	size_t size;
	size_t capacity;
	GenType* ptrInternal;

};


/*---------------------*/



template <class NodeType, class ElementType>
class TDoubleLinkedListIterator
{
public:

	explicit TDoubleLinkedListIterator(NodeType* StartingNode)
		: CurrentNode(StartingNode)
	{ }

	/** conversion to "bool" returning true if the iterator is valid. */
	FORCEINLINE explicit operator bool() const
	{
		return CurrentNode != nullptr;
	}

	TDoubleLinkedListIterator& operator++()
	{
		CurrentNode = CurrentNode->GetNextNode();
		return *this;
	}

	TDoubleLinkedListIterator operator++(int)
	{
		auto Tmp = *this;
		++(*this);
		return Tmp;
	}

	TDoubleLinkedListIterator& operator--()
	{
		CurrentNode = CurrentNode->GetPrevNode();
		return *this;
	}

	TDoubleLinkedListIterator operator--(int)
	{
		auto Tmp = *this;
		--(*this);
		return Tmp;
	}

	// Accessors.
	ElementType& operator->() const
	{
		return CurrentNode->GetValue();
	}

	ElementType& operator*() const
	{
		return CurrentNode->GetValue();
	}

	NodeType* GetNode() const
	{
		return CurrentNode;
	}

private:
	NodeType* CurrentNode;

	friend bool operator==(const TDoubleLinkedListIterator& Lhs, const TDoubleLinkedListIterator& Rhs) { return Lhs.CurrentNode == Rhs.CurrentNode; }
	friend bool operator!=(const TDoubleLinkedListIterator& Lhs, const TDoubleLinkedListIterator& Rhs) { return Lhs.CurrentNode != Rhs.CurrentNode; }
};


/**
 * Double linked list.
 */
template <class ElementType>
class TDoubleLinkedList
{
public:
	class TDoubleLinkedListNode
	{
	public:
		friend class TDoubleLinkedList;

		/** Constructor */
		TDoubleLinkedListNode(const ElementType& InValue)
			: Value(InValue), NextNode(nullptr), PrevNode(nullptr)
		{ }

		const ElementType& GetValue() const
		{
			return Value;
		}

		ElementType& GetValue()
		{
			return Value;
		}

		TDoubleLinkedListNode* GetNextNode()
		{
			return NextNode;
		}

		const TDoubleLinkedListNode* GetNextNode() const
		{
			return NextNode;
		}

		TDoubleLinkedListNode* GetPrevNode()
		{
			return PrevNode;
		}

		const TDoubleLinkedListNode* GetPrevNode() const
		{
			return PrevNode;
		}

	protected:
		ElementType            Value;
		TDoubleLinkedListNode* NextNode;
		TDoubleLinkedListNode* PrevNode;
	};

	/**
	 * Used to iterate over the elements of a linked list.
	 */
	typedef TDoubleLinkedListIterator<TDoubleLinkedListNode, ElementType> TIterator;
	typedef TDoubleLinkedListIterator<TDoubleLinkedListNode, const ElementType> TConstIterator;

	/** Constructors. */
	TDoubleLinkedList()
		: HeadNode(nullptr)
		, TailNode(nullptr)
		, ListSize(0)
	{ }

	/** Destructor */
	virtual ~TDoubleLinkedList()
	{
		Empty();
	}

	// Adding/Removing methods

	/**
	 * Add the specified value to the beginning of the list, making that value the new head of the list.
	 *
	 * @param	InElement	the value to add to the list.
	 * @return	whether the node was successfully added into the list.
	 * @see GetHead, InsertNode, RemoveNode
	 */
	bool AddHead(const ElementType& InElement)
	{
		return AddHead(new TDoubleLinkedListNode(InElement));
	}

	bool AddHead(TDoubleLinkedListNode* NewNode)
	{
		if (NewNode == nullptr)
		{
			return false;
		}

		// have an existing head node - change the head node to point to this one
		if (HeadNode != nullptr)
		{
			NewNode->NextNode = HeadNode;
			HeadNode->PrevNode = NewNode;
			HeadNode = NewNode;
		}
		else
		{
			HeadNode = TailNode = NewNode;
		}

		SetListSize(ListSize + 1);
		return true;
	}

	/**
	 * Append the specified value to the end of the list
	 *
	 * @param	InElement	the value to add to the list.
	 * @return	whether the node was successfully added into the list
	 * @see GetTail, InsertNode, RemoveNode
	 */
	bool AddTail(const ElementType& InElement)
	{
		return AddTail(new TDoubleLinkedListNode(InElement));
	}

	bool AddTail(TDoubleLinkedListNode* NewNode)
	{
		if (NewNode == nullptr)
		{
			return false;
		}

		if (TailNode != nullptr)
		{
			TailNode->NextNode = NewNode;
			NewNode->PrevNode = TailNode;
			TailNode = NewNode;
		}
		else
		{
			HeadNode = TailNode = NewNode;
		}

		SetListSize(ListSize + 1);
		return true;
	}

	/**
	 * Insert the specified value into the list at an arbitrary point.
	 *
	 * @param	InElement			the value to insert into the list
	 * @param	NodeToInsertBefore	the new node will be inserted into the list at the current location of this node
	 *								if nullptr, the new node will become the new head of the list
	 * @return	whether the node was successfully added into the list
	 * @see Empty, RemoveNode
	 */
	bool InsertNode(const ElementType& InElement, TDoubleLinkedListNode* NodeToInsertBefore = nullptr)
	{
		return InsertNode(new TDoubleLinkedListNode(InElement), NodeToInsertBefore);
	}

	bool InsertNode(TDoubleLinkedListNode* NewNode, TDoubleLinkedListNode* NodeToInsertBefore = nullptr)
	{
		if (NewNode == nullptr)
		{
			return false;
		}

		if (NodeToInsertBefore == nullptr || NodeToInsertBefore == HeadNode)
		{
			return AddHead(NewNode);
		}

		NewNode->PrevNode = NodeToInsertBefore->PrevNode;
		NewNode->NextNode = NodeToInsertBefore;

		NodeToInsertBefore->PrevNode->NextNode = NewNode;
		NodeToInsertBefore->PrevNode = NewNode;

		SetListSize(ListSize + 1);
		return true;
	}

	/**
	 * Remove the node corresponding to InElement.
	 *
	 * @param InElement The value to remove from the list.
	 * @see Empty, InsertNode
	 */
	void RemoveNode(const ElementType& InElement)
	{
		TDoubleLinkedListNode* ExistingNode = FindNode(InElement);
		RemoveNode(ExistingNode);
	}

	/**
	 * Removes the node specified.
	 *
	 * @param NodeToRemove The node to remove.
	 * @see Empty, InsertNode
	 */
	void RemoveNode(TDoubleLinkedListNode* NodeToRemove, bool bDeleteNode = true)
	{
		if (NodeToRemove != nullptr)
		{
			// if we only have one node, just call Clear() so that we don't have to do lots of extra checks in the code below
			if (Num() == 1)
			{
				if (bDeleteNode)
				{
					Empty();
				}
				else
				{
					NodeToRemove->NextNode = NodeToRemove->PrevNode = nullptr;
					HeadNode = TailNode = nullptr;
					SetListSize(0);
				}
				return;
			}

			if (NodeToRemove == HeadNode)
			{
				HeadNode = HeadNode->NextNode;
				HeadNode->PrevNode = nullptr;
			}

			else if (NodeToRemove == TailNode)
			{
				TailNode = TailNode->PrevNode;
				TailNode->NextNode = nullptr;
			}
			else
			{
				NodeToRemove->NextNode->PrevNode = NodeToRemove->PrevNode;
				NodeToRemove->PrevNode->NextNode = NodeToRemove->NextNode;
			}

			if (bDeleteNode)
			{
				delete NodeToRemove;
			}
			else
			{
				NodeToRemove->NextNode = NodeToRemove->PrevNode = nullptr;
			}
			SetListSize(ListSize - 1);
		}
	}

	/** Removes all nodes from the list. */
	void Empty()
	{
		TDoubleLinkedListNode* Node;
		while (HeadNode != nullptr)
		{
			Node = HeadNode->NextNode;
			delete HeadNode;
			HeadNode = Node;
		}

		HeadNode = TailNode = nullptr;
		SetListSize(0);
	}

	// Accessors.

	/**
	 * Returns the node at the head of the list.
	 *
	 * @return Pointer to the first node.
	 * @see GetTail
	 */
	TDoubleLinkedListNode* GetHead() const
	{
		return HeadNode;
	}

	/**
	 * Returns the node at the end of the list.
	 *
	 * @return Pointer to the last node.
	 * @see GetHead
	 */
	TDoubleLinkedListNode* GetTail() const
	{
		return TailNode;
	}

	/**
	 * Finds the node corresponding to the value specified
	 *
	 * @param	InElement	the value to find
	 * @return	a pointer to the node that contains the value specified, or nullptr of the value couldn't be found
	 */
	TDoubleLinkedListNode* FindNode(const ElementType& InElement)
	{
		TDoubleLinkedListNode* Node = HeadNode;
		while (Node != nullptr)
		{
			if (Node->GetValue() == InElement)
			{
				break;
			}

			Node = Node->NextNode;
		}

		return Node;
	}

	bool Contains(const ElementType& InElement)
	{
		return (FindNode(InElement) != nullptr);
	}

	/**
	 * Returns the number of items in the list.
	 *
	 * @return Item count.
	 */
	int32 Num() const
	{
		return ListSize;
	}

protected:

	/**
	 * Updates the size reported by Num().  Child classes can use this function to conveniently
	 * hook into list additions/removals.
	 *
	 * @param	NewListSize		the new size for this list
	 */
	virtual void SetListSize(int32 NewListSize)
	{
		ListSize = NewListSize;
	}

private:
	TDoubleLinkedListNode* HeadNode;
	TDoubleLinkedListNode* TailNode;
	int32 ListSize;

	TDoubleLinkedList(const TDoubleLinkedList&);
	TDoubleLinkedList& operator=(const TDoubleLinkedList&);

	friend TIterator      begin(TDoubleLinkedList& List) { return TIterator(List.GetHead()); }
	friend TConstIterator begin(const TDoubleLinkedList& List) { return TConstIterator(List.GetHead()); }
	friend TIterator      end(TDoubleLinkedList& List) { return TIterator(nullptr); }
	friend TConstIterator end(const TDoubleLinkedList& List) { return TConstIterator(nullptr); }
};


#include "FArray.inl"