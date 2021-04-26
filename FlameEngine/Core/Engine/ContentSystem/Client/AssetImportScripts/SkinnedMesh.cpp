#include "SkinnedMesh.h"


/*
*	vLength
*	vertexSize
*	iLength
*	indexType
*	vdeclLength
*
*	vdecl
*	vdata
*	idata
*/


struct FMeshGeometryHeader
{
	uint64 vLength;
	uint64 elementSize;
	uint64 iLength;
	EFRIIndexType indexType;
	uint32 vdeclLength;
};


struct FSkeletonDataHeader
{
	uint32 JointCount;
	uint32 RootJointIndex;
};


struct FJointData
{
	uint32 Id;
	FAnsiString Name;
	FMatrix4 bindTransform;
	FArray<uint32> childrenIndices;

	FJointData()
	{}

	FJointData(const FJointData& other) :
		Id(other.Id),
		Name(other.Name),
		bindTransform(other.bindTransform),
		childrenIndices(other.childrenIndices)
	{}

};


FORCEINLINE size_t SwitchIndexType(EFRIIndexType type)
{
	if (type == EFRIIndexType::UInt8) return sizeof(byte);
	else if (type == EFRIIndexType::UInt16) return sizeof(uint16_t);
	else if (type == EFRIIndexType::UInt32) return sizeof(uint32);
	else return 0;
}

Joint* CreateJointSkeletonTree(FArray<FJointData>& jointData, uint32 nodeJointIndex)
{
	FJointData& j = jointData[nodeJointIndex];
	Joint* rootJoint = new Joint(nodeJointIndex, j.Name, j.bindTransform);

	for (int i = 0; i < j.childrenIndices.Length(); i++)
	{
		rootJoint->AddChildJoint(CreateJointSkeletonTree(jointData, j.childrenIndices[i]));
	}

	rootJoint->CalculateInverseBind(FMatrix4(1));

	return rootJoint;
}



SkinnedMeshComponent FSkinnedMeshSerializer::Serialize(IOFileStream& fileStream)
{

	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	FResourceVertexBuffer* vBuffer;
	FResourceIndexBuffer* iBuffer;
	uint32 vertexNumber;
	uint32 elementNumber;
	FArray<byte> vData;
	FArray<byte> iData;
	FMeshGeometryHeader geomHeader = fileStream.Read<FMeshGeometryHeader>();
	FArray<FResourceVertexDeclarationComponent> vDecl(geomHeader.vdeclLength);

	fileStream.ReadArray<FResourceVertexDeclarationComponent>(vDecl);

	vData.Resize(geomHeader.vLength * geomHeader.elementSize);
	iData.Resize(geomHeader.iLength * SwitchIndexType(geomHeader.indexType));


	fileStream.ReadArray<byte>(vData);
	fileStream.ReadArray<byte>(iData);

	vBuffer = allocator->DynamicCreateVertexBuffer(geomHeader.vLength, 0, FResourceCreationDescriptor((FResourceArrayInterface*)vData.Begin(), vData.ByteSize()));
	iBuffer = allocator->DynamicCreateIndexBuffer(geomHeader.iLength, 0, FResourceCreationDescriptor((FResourceArrayInterface*)iData.Begin(), iData.ByteSize()));

	allocator->AttachVertexDeclaration(vDecl);


	FArray<FJointData> jointData;
	FSkeletonDataHeader skelHeader = fileStream.Read<FSkeletonDataHeader>();

	for (uint32 i = 0; i < skelHeader.JointCount; i++)
	{
		FJointData joint;

		joint.Id = fileStream.Read<uint32>();
		joint.Name = fileStream.ReadLengthedString<FAnsiString>();
		joint.bindTransform = fileStream.Read<FMatrix4>();
		
		uint32 childrenCount = fileStream.Read<uint32>();
		joint.childrenIndices = FArray<uint32>(childrenCount);
		fileStream.ReadArray<uint32>(joint.childrenIndices);


		jointData.Add(joint);
	}

	SkeletalComponent skeleton(CreateJointSkeletonTree(jointData, skelHeader.RootJointIndex), skelHeader.JointCount);

	return SkinnedMeshComponent(vBuffer, iBuffer, vDecl, skeleton);
}