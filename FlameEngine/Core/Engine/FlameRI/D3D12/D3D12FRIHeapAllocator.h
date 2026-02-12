#pragma once

class FD3D12ShaderResourceView;

class FD3D12OfflineDescriptorHeap : public FD3D12DeviceChild
{
	struct HeapRange
	{
		SIZE_T Start;
		SIZE_T End;
	};
	struct HeapNode
	{
		ComPtr<ID3D12DescriptorHeap> pHeap;
		TDoubleLinkedList<HeapRange> FreeRanges;

		HeapNode() {}
	};

	FArray<HeapNode> Heaps;
	D3D12_DESCRIPTOR_HEAP_DESC Descriptor;
	uint32 DescriptorSize;
	TDoubleLinkedList<uint32> FreeHeaps;

public:
	FD3D12OfflineDescriptorHeap(FD3D12Device* InDevice, D3D12_DESCRIPTOR_HEAP_TYPE HeapType, uint32 HeapSize) : 
		FD3D12DeviceChild(InDevice),
		DescriptorSize(0)
	{
		Descriptor = {};
		Descriptor.NumDescriptors = HeapSize;
		Descriptor.Type = HeapType;
		Descriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DescriptorSize = InDevice->GetDevice()->GetDescriptorHandleIncrementSize(HeapType);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE AllocateHeapSlot(uint32& outIndex)
	{
		printf("Creating descriptor slot %p\n", this);

		if (FreeHeaps.Num() == 0)
		{
			printf("Allocating new heap %p\n", this);
			AllocateHeap();
		}
		auto Head = FreeHeaps.GetHead();
		outIndex = Head->GetValue();
		HeapNode& HeapEntry = Heaps[outIndex];

		HeapRange& Range = HeapEntry.FreeRanges.GetHead()->GetValue();
		D3D12_CPU_DESCRIPTOR_HANDLE Ret = { Range.Start };
		Range.Start += DescriptorSize;

		if (Range.Start == Range.End)
		{
			HeapEntry.FreeRanges.RemoveNode(HeapEntry.FreeRanges.GetHead());
			if (0 == HeapEntry.FreeRanges.Num())
			{
				FreeHeaps.RemoveNode(Head);
			}
		}
		return Ret;
	}

	void FreeHeapSlot(D3D12_CPU_DESCRIPTOR_HANDLE Offset, uint32 index)
	{
		HeapNode& HeapEntry = Heaps[index];

		HeapRange NewRange =
		{
			Offset.ptr,
			Offset.ptr + DescriptorSize
		};

		bool bFound = false;
		for (auto Node = HeapEntry.FreeRanges.GetHead();
			Node != nullptr && !bFound;
			Node = Node->GetNextNode())
		{
			HeapRange& Range = Node->GetValue();
			if (Range.Start == Offset.ptr + DescriptorSize)
			{
				Range.Start = Offset.ptr;
				bFound = true;
			}
			else if (Range.End == Offset.ptr)
			{
				Range.End += DescriptorSize;
				bFound = true;
			}
			else
			{
				if (Range.Start > Offset.ptr)
				{
					HeapEntry.FreeRanges.InsertNode(NewRange, Node);
					bFound = true;
				}
			}
		}

		if (!bFound)
		{
			if (0 == HeapEntry.FreeRanges.Num())
			{
				FreeHeaps.AddTail(index);
			}
			HeapEntry.FreeRanges.AddTail(NewRange);
		}
	}

private:
	void AllocateHeap()
	{
		ComPtr<ID3D12DescriptorHeap> Heap;
		ThrowIfFailed(GetParentDevice()->GetDevice()->CreateDescriptorHeap(&Descriptor, IID_PPV_ARGS(Heap.GetAddressOf())));

		D3D12_CPU_DESCRIPTOR_HANDLE HeapBase = Heap->GetCPUDescriptorHandleForHeapStart();
		// Allocate and initialize a single new entry in the map

		Heaps.Resize(Heaps.Length() + 1);
		new (Heaps.End() - 1) HeapNode();
		HeapNode& HeapEntry = Heaps.Last();
		HeapEntry.FreeRanges.AddTail({ 
			HeapBase.ptr,
			HeapBase.ptr + Descriptor.NumDescriptors * DescriptorSize 
			});
		HeapEntry.pHeap = Heap;
		FreeHeaps.AddTail(Heaps.Length() - 1);
	}

};



class FD3D12OnlineDescriptorHeap : public FD3D12DeviceChild
{

	ComPtr<ID3D12DescriptorHeap> pHeap;
	D3D12_DESCRIPTOR_HEAP_DESC Descriptor;

	D3D12_GPU_DESCRIPTOR_HANDLE GPUBase;
	D3D12_CPU_DESCRIPTOR_HANDLE CPUBase;
	uint64 DescriptorSize;

public:
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSlotHandle(uint32 Slot) const { return{ CPUBase.ptr + Slot * DescriptorSize }; }
	FORCEINLINE D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSlotHandle(uint32 Slot) const { return{ GPUBase.ptr + Slot * DescriptorSize }; }
	FORCEINLINE uint64 GetDescriptorSize() const { return DescriptorSize; }
	FORCEINLINE ID3D12DescriptorHeap* GetHeap() { return pHeap.Get(); }

	FD3D12OnlineDescriptorHeap(FD3D12Device* InDevice, D3D12_DESCRIPTOR_HEAP_TYPE HeapType, uint32 HeapSize) :
		FD3D12DeviceChild(InDevice)
	{
		Descriptor = {};
		Descriptor.NumDescriptors = HeapSize;
		Descriptor.Type = HeapType;
		Descriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DescriptorSize = InDevice->GetDevice()->GetDescriptorHandleIncrementSize(HeapType);

		ThrowIfFailed(GetParentDevice()->GetDevice()->CreateDescriptorHeap(&Descriptor, IID_PPV_ARGS(pHeap.GetAddressOf())));

		CPUBase = pHeap->GetCPUDescriptorHandleForHeapStart();
		GPUBase = pHeap->GetGPUDescriptorHandleForHeapStart();

	}
};

