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

	FJointData() :
		Id(0)	
	{}

	FJointData(const FJointData& other) :
		Id(other.Id),
		Name(other.Name),
		bindTransform(other.bindTransform),
		childrenIndices(other.childrenIndices)
	{}

};

Joint* CreateJointSkeletonTree(FArray<FJointData>& jointData, uint32 nodeJointIndex)
{
	FJointData& j = jointData[nodeJointIndex];
	Joint* rootJoint = new Joint(nodeJointIndex, j.Name, j.bindTransform);

	for (int i = 0; i < j.childrenIndices.Length(); i++)
	{
		rootJoint->AddChildJoint(CreateJointSkeletonTree(jointData, j.childrenIndices[i]));
	}

	return rootJoint;
}

TContentSerializer<SkinnedMesh>::TContentSerializer(FRIContext* context) :
	renderContext(context)
{
	FRICommandList cmdList(context->GetFRIDynamic(), true);

	FRIVertexShader* signatureShader = NULL;
	if (renderContext->RenderFramework == EFRIRendererFramework::DX11)
	{
		signatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("shaders/signature/dx/bin/SkinnedMesh.signature.cso").ReadBytes());
	}

	FArray<FRIVertexDeclarationComponent> DeclCompArray;
	DeclCompArray.Add(FRIVertexDeclarationComponent("POSITION",		3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 88, 0));
	DeclCompArray.Add(FRIVertexDeclarationComponent("NORMAL",			3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 88, 12));
	DeclCompArray.Add(FRIVertexDeclarationComponent("TANGENT",		3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 88, 24));
	DeclCompArray.Add(FRIVertexDeclarationComponent("BITANGENT",		3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 88, 36));
	DeclCompArray.Add(FRIVertexDeclarationComponent("TEXCOORD",		2, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 88, 48));
	DeclCompArray.Add(FRIVertexDeclarationComponent("JOINT_INDICES",	4, EFRIVertexDeclerationAttributeType::Int,	  EFRIBool::False, 88, 56));
	DeclCompArray.Add(FRIVertexDeclarationComponent("JOINT_WEIGHTS",	4, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 88, 72));

	FArray<FRIVertexDeclarationDesc> VLayouts;
	VLayouts.Add(FRIVertexDeclarationDesc(DeclCompArray, 0));

	vertexDeclaration = cmdList.GetDynamic()->CreateVertexDeclaration(VLayouts, signatureShader);

	delete signatureShader;
}


SkinnedMesh TContentSerializer<SkinnedMesh>::Serialize(IOFileStream& fileStream)
{

	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	FRIVertexBuffer* vBuffer;
	FRIIndexBuffer* iBuffer;
	uint32 vertexNumber;
	uint32 elementNumber;
	FArray<byte> vData;
	FArray<byte> iData;
	FMeshGeometryHeader geomHeader = fileStream.Read<FMeshGeometryHeader>();


	vData.Resize(geomHeader.vLength * geomHeader.elementSize);
	iData.Resize(geomHeader.iLength * sizeof(uint32));


	fileStream.ReadArray<byte>(vData);
	fileStream.ReadArray<byte>(iData);

	vBuffer = allocator->CreateVertexBuffer(geomHeader.vLength, 0, FRICreationDescriptor((FRIArrayInterface*)vData.Begin(), vData.ByteSize()));
	iBuffer = allocator->CreateIndexBuffer(geomHeader.iLength, 0, FRICreationDescriptor((FRIArrayInterface*)iData.Begin(), iData.ByteSize()));

	allocator->AttachVertexDeclaration(vBuffer, vertexDeclaration);


	FArray<FJointData> jointData;
	FSkeletonDataHeader skelHeader = fileStream.Read<FSkeletonDataHeader>();

	//printf("Inverse bind Transforms:\n");


	for (uint32 i = 0; i < skelHeader.JointCount; i++)
	{
		FJointData joint;

		joint.Id = fileStream.Read<uint32>();
		joint.Name = fileStream.ReadLengthedString<FAnsiString>();
		joint.bindTransform = fileStream.Read<FMatrix4>();
		
		//printf("Joint index: %d\n", i);
		//PrintDebugMatrix3(FMatrix4::Inverse(joint.bindTransform));

		uint32 childrenCount = fileStream.Read<uint32>();
		joint.childrenIndices = FArray<uint32>(childrenCount);
		fileStream.ReadArray<uint32>(joint.childrenIndices);


		jointData.Add(joint);
	}

	Skeleton skeleton(CreateJointSkeletonTree(jointData, skelHeader.RootJointIndex), skelHeader.JointCount);

	return SkinnedMesh(vBuffer, iBuffer, skeleton);
}