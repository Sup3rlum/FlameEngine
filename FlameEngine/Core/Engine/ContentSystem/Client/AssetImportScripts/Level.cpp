#include "Level.h"

#include "Material.h"
#include "Core/Engine/GameSystem/Common/Scene.h"
#include "../LocalAssetManager.h"

struct FLevelStaticGeometryHeader
{
	uint32 MaterialID;
	uint32 dummy;

	uint64 vLength;
	uint64 elementSize;
	uint64 iLength;
};

TContentSerializer<Level>::TContentSerializer(FRIContext* context, Scene* sc)
	: renderContext(context),
	scene(sc)
{
	FRICommandList cmdList(context->GetFRIDynamic(), true);

	FRIVertexShader* signatureShader = NULL;
	if (renderContext->RenderFramework == EFRIRendererFramework::DX11)
	{
		signatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("Assets/Shaders/signature/dx/bin/Mesh.signature.cso").ReadBytes());
	}

	FArray<FRIVertexDeclarationComponent> DeclCompArray;
	DeclCompArray.Add(FRIVertexDeclarationComponent("POSITION", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 0));
	DeclCompArray.Add(FRIVertexDeclarationComponent("NORMAL", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 12));
	DeclCompArray.Add(FRIVertexDeclarationComponent("TANGENT", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 24));
	DeclCompArray.Add(FRIVertexDeclarationComponent("BITANGENT", 3, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 36));
	DeclCompArray.Add(FRIVertexDeclarationComponent("TEXCOORD", 2, EFRIVertexDeclerationAttributeType::Float, EFRIBool::False, 56, 48));


	FArray<FRIVertexDeclarationDesc> LayoutDesc;
	LayoutDesc.Add(FRIVertexDeclarationDesc(DeclCompArray, 0));

	vertexDeclaration = cmdList.GetDynamic()->CreateVertexDeclaration(LayoutDesc, signatureShader);

	delete signatureShader;
}

Level TContentSerializer<Level>::Serialize(IOFileStream& fileStream)
{
	Level level;

	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	uint32 NumMaterials = fileStream.Read<uint32>();

	FArray<Material> Materials;

	FAssetManager Content;
	Content.Connect("./Assets/");

	for (int i = 0; i < NumMaterials; i++)
	{
		FString8 materialName = fileStream.ReadLengthedString<FString8>();
		Materials.Add(Content.Load<Material>(materialName.ToPlatformString(), renderContext));
	}


	level.LevelGeometry.Root = new IBVHNode<2, StaticGeometry>();

	uint32 NumSGBrushes = fileStream.Read<uint32>();
	for (int i = 0; i < NumSGBrushes; i++)
	{

		FArray<byte> vData;
		FArray<uint32> iData;
		FLevelStaticGeometryHeader geomHeader = fileStream.Read<FLevelStaticGeometryHeader>();

		vData.Resize(geomHeader.vLength * geomHeader.elementSize);
		iData.Resize(geomHeader.iLength);

		fileStream.ReadArray<byte>(vData);
		fileStream.ReadArray<uint32>(iData);


		PhysicsTriangleMeshDesc triangleMeshDesc;
		triangleMeshDesc.DataStride = geomHeader.elementSize;
		triangleMeshDesc.VertexData = vData;
		triangleMeshDesc.IndexData = iData;

		for (int i = 0; i < vData.Length(); i += geomHeader.elementSize)
		{
			FVector3 vec = *(FVector3*)(&vData[i]);

			level.LevelGeometry.Root->BoundingBox.Enclose(vec);
		}


		auto brush = new StaticGeometry(scene->Physics->CreateStatic(FTransform()));
		brush->PhysicsBody.SetShape(PhysicsShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), scene->Physics->CookTriangleMeshGeometry(triangleMeshDesc)));

		brush->VertexBuffer = allocator->CreateVertexBuffer(geomHeader.vLength, 0, FRICreationDescriptor((FRIArrayInterface*)vData.Begin(), vData.ByteSize()));
		brush->IndexBuffer = allocator->CreateIndexBuffer(geomHeader.iLength, 0, FRICreationDescriptor((FRIArrayInterface*)iData.Begin(), iData.ByteSize()));
		allocator->AttachVertexDeclaration(brush->VertexBuffer, vertexDeclaration);

		brush->Material = Materials[geomHeader.MaterialID];

		level.LevelGeometry.Leafs.Add(brush);
	}



	return level;
}