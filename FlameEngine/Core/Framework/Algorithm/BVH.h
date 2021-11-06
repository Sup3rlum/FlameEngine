#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"


template<size_t Height, typename TLeaf>
struct IBVHNode
{
	typedef IBVHNode<Height,TLeaf> BVHNodeType;

	AABB BoundingBox;
	FStaticArray<BVHNodeType*, Height> Children;
	TLeaf* Leaf;


	IBVHNode() : 
		Leaf(NULL)
	{
		Memory::Zero(Children.Begin(), Children.ByteSize());
	}


	bool IsLeaf() const
	{
		return Leaf != NULL;
	}



	TLeaf* RayCast(FRay ray)
	{
		// If this node is marked as a leaf, return its contents
		if (IsLeaf())
			return Leaf;

		// Otherwise check all children
		for (int i = 0; i < Height; i++)
		{
			auto Child = Children[i];
			if (Child && Child->BoundingBox.RayCast(ray))
			{
				// If ray intersects a child, repeat process for that node
				return Child->RayCast(ray);
			}
		}

		// If node is not a leaf and no children have been hit, the node is either an empty leaf, or the ray passes inbetween AABB's

		return NULL;
	}

};

template<size_t Order, typename TLeaf>
class IBVHierarchy
{

public:
	typedef IBVHNode<Order, TLeaf> BVHNodeType;

	FArray<TLeaf*> Leafs;
	BVHNodeType* Root;

	TLeaf* RayCast(FRay ray)
	{

		if (Root->BoundingBox.RayCast(ray))
		{
			return Root->RayCast(ray);
		}

		return NULL;
	}

	IBVHierarchy() :
		Root(NULL)
	{}

	IBVHierarchy(const IBVHierarchy& other) :
		Root(other.Root),
		Leafs(other.Leafs)
	{}

	IBVHierarchy(IBVHierarchy&& other) :
		Root(other.Root),
		Leafs(MoveRef(other.Leafs))
	{}


	IBVHierarchy& operator=(const IBVHierarchy& other)
	{
		Root = other.Root;
		Leafs = other.Leafs;

		return *this;
	}

	FORCEINLINE uint32 GetOrder() const
	{
		return Order;
	}
	
	static IBVHierarchy ConstructFromArray(FArray<TLeaf*> leafs, FArray<AABB> boundingVolumes)
	{
		IBVHierarchy Bvh;
		
	}

};