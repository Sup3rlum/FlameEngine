#include "RiggedMesh.h"
#include "Core/Engine/Renderer/Common/Geometry/VertexComponent.h"

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
	FString8 Name;
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
	Joint* rootJoint = new Joint(nodeJointIndex, j.Name, FMatrix4::Inverse(j.bindTransform));

	for (int i = 0; i < j.childrenIndices.Length(); i++)
	{
		rootJoint->AddChildJoint(CreateJointSkeletonTree(jointData, j.childrenIndices[i]));
	}

	return rootJoint;
}

TContentSerializer<RiggedMesh>::TContentSerializer(IAssetManager* manager) :
	renderContext(manager->RenderContext)
{
	/*FRICommandList cmdList(renderContext->GetFRIDynamic(), true);

	FRIVertexShader* signatureShader = NULL;
	if (renderContext->InstanceDescription.RenderFramework == EFRIRendererFramework::DX11)
	{
		signatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("Assets/Shaders/signature/dx/bin/SkinnedMesh.signature.cso").ReadBytes());
	}

	vertexDeclaration = cmdList.GetDynamic()->CreateInputLayout(
		{ FRIInputDesc(InputLayouts::SkinnedLit, 0) },
		signatureShader);

	delete signatureShader;*/
}


RiggedMesh TContentSerializer<RiggedMesh>::Serialize(Stream& fileStream)
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


	fileStream.Read(vData);
	fileStream.Read(iData);

	vBuffer = allocator->CreateVertexBuffer(geomHeader.vLength, geomHeader.elementSize, EFRIAccess::None, EFRIUsage::Default, FRICreationDescriptor(vData.Begin(), vData.ByteSize()));
	iBuffer = allocator->CreateIndexBuffer(geomHeader.iLength, EFRIAccess::None, EFRIUsage::Default, FRICreationDescriptor(iData.Begin(), iData.ByteSize()));

	//allocator->AttachInputLayout(vBuffer, vertexDeclaration);


	FArray<FJointData> jointData;
	FSkeletonDataHeader skelHeader = fileStream.Read<FSkeletonDataHeader>();


	for (uint32 i = 0; i < skelHeader.JointCount; i++)
	{
		FJointData joint;

		joint.Id = fileStream.Read<uint32>();
		joint.Name = fileStream.ReadLengthedString<FString8>();
		joint.bindTransform = fileStream.Read<FMatrix4>();
		
		uint32 childrenCount = fileStream.Read<uint32>();
		joint.childrenIndices = FArray<uint32>(childrenCount);
		fileStream.Read(joint.childrenIndices);


		jointData.Add(joint);
	}

	Skeleton skeleton(CreateJointSkeletonTree(jointData, skelHeader.RootJointIndex), skelHeader.JointCount);

	return RiggedMesh(vBuffer, iBuffer, skeleton);
}