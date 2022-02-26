#pragma once

#include "Core/Common/CoreBase.h"
#include "Core/Common/TypeTraits.h"
#include "Core/Runtime/Common/Memory.h"

struct FRange
{

	struct Iterator
	{
	public:

		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = int64;
		using pointer = int64*;  
		using reference = int64&;
		using const_reference = const int64&; 

		reference operator *() { return value; }
		const_reference operator *() const { return value; }

		const Iterator& operator++()
		{ 
			forward ? ++value : --value;
			return *this; 
		}
		Iterator operator ++(int) 
		{ 
			Iterator copy(*this); 
			forward ? ++value : --value;
			return copy; 
		}

		const Iterator& operator--() 
		{ 
			forward ? --value : ++value;
			return *this; 
		}
		Iterator operator --(int) 
		{ 
			Iterator copy(*this); 
			forward ? --value : ++value;
			return copy; 
		}

		operator int64()
		{
			return value;
		}

		bool operator ==(const Iterator& other) const { return value == other.value; }
		bool operator !=(const Iterator& other) const { return value != other.value; }

		Iterator() : value(0), forward(true) {}
		Iterator(int64 value, bool forward) : value(value), forward(forward) {}
		Iterator(const Iterator& other) : value(other.value), forward(other.forward) {}

	private:
		bool forward;
		int64 value;
	};

	FRange(int64 start, int64 end) :
		Start(start, end-start > 0),
		End(end, end - start > 0)
	{}

	FRange(int64 end) :
		Start(0, end > 0),
		End(end, end > 0)
	{}

	Iterator begin() const { return Start; }
	Iterator end() const { return End; }

private:
	Iterator Start, End;
};