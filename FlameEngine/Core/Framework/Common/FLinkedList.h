#pragma once

template<typename Ty>
struct FLinkedListNode
{
	typedef Ty _Type;

	FLinkedListNode(const Ty& value) :
		Value(value)
	{
	}
	~FLinkedListNode()
	{
		if (Next)
		{
			delete Next;
		}
	}

	/*operator Ty()
	{
		return value;
	}*/

	Ty Value;
	FLinkedListNode* Next = NULL;
};


template<typename Ty>
struct FLinkedList
{
	typedef FLinkedListNode<Ty> _ListNode;


	FLinkedList()
	{
		Top = &Root;
	}

	Ty At(size_t index)
	{
		//assert(index < Length);

		int counter = 0;
		auto node = Root;

		while (node)
		{
			if (counter == index)
			{
				return node->Value;
			}
			counter++;
			node = Root->Next;
		}
	}

	void Clear()
	{
		delete Root;
		Root = NULL;
		Top = &Root;

		Length = 0;
	}

	void InsertTop(const Ty& value)
	{
		//assert(*Top == NULL);

		if (Top == NULL)
			Top = &Root;

		auto NewTop = new _ListNode(value);
		*Top = NewTop;
		Top = &(NewTop->Next);
		Length++;
	}

	void InsertBottom(const Ty& value)
	{
		auto NewRoot = new _ListNode(value);
		NewRoot->Next = Root;
		Root = NewRoot;
		Length++;
	}

	_ListNode* Root = NULL;
	_ListNode** Top = NULL;
	size_t Length = 0;
};