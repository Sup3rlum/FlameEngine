#include "Level.h"

#include "Material.h"
#include "Core/Engine/GameSystem/Common/Scene.h"
#include "../Client/LocalAssetManager.h"
#include "Core/Engine/Renderer/Common/Geometry/VertexComponent.h"

struct FLevelStaticGeometryHeader
{
	uint32 MaterialID;
	uint32 dummy;

	uint64 vLength;
	uint64 elementSize;
	uint64 iLength;
};

TContentSerializer<Level>::TContentSerializer(IAssetManager* manager, Scene* sc) : 
	renderContext(manager->RenderContext),
	scene(sc),
	Content(manager)
{
	/*FRICommandList cmdList(renderContext->GetFRIDynamic(), true);

	FRIVertexShader* signatureShader = NULL;
	if (renderContext->InstanceDescription.RenderFramework == EFRIRendererFramework::DX11)
	{
		signatureShader = cmdList.GetDynamic()->CreateVertexShader(IOFileStream("Assets/Shaders/signature/dx/bin/Mesh.signature.cso").ReadBytes());
	}

	vertexDeclaration = cmdList.GetDynamic()->CreateInputLayout(
		{ FRIInputDesc(InputLayouts::StaticLit, 0) }, 
		signatureShader);

	delete signatureShader;*/
}

Level TContentSerializer<Level>::Serialize(Stream& fileStream)
{
	Level level;

	FRIDynamicAllocator* allocator = renderContext->GetFRIDynamic();
	uint32 NumMaterials = fileStream.Read<uint32>();

	FArray<Material> Materials;
	FRICommandList cmdList(renderContext->GetCommandContext(0));



	for (int i = 0; i < NumMaterials; i++)
	{
		FString8 materialName = fileStream.ReadLengthedString<FString8>();
		Materials.Add(((FAssetManager*)Content)->Load<Material>(materialName.ToPlatformString()));
	}

	cmdList.Open();
	level.LevelGeometry.Root = new IBVHNode<2, StaticGeometry>();

	uint32 NumSGBrushes = fileStream.Read<uint32>();
	for (int i = 0; i < NumSGBrushes; i++)
	{

		FArray<byte> vData;
		FArray<uint32> iData;
		FLevelStaticGeometryHeader geomHeader = fileStream.Read<FLevelStaticGeometryHeader>();

		vData.Resize(geomHeader.vLength * geomHeader.elementSize);
		iData.Resize(geomHeader.iLength);

		fileStream.Read<byte>(vData);
		fileStream.Read<uint32>(iData);

		FRICreationDescriptor vertexDesc(vData.Begin(), vData.ByteSize());
		FRICreationDescriptor indexDesc(iData.Begin(), iData.ByteSize());

		PhysicsTriangleMeshDesc triangleMeshDesc;
		triangleMeshDesc.DataStride = geomHeader.elementSize;
		triangleMeshDesc.VertexData = vertexDesc;
		triangleMeshDesc.IndexData = indexDesc;

		for (int i = 0; i < vData.Length(); i += geomHeader.elementSize)
		{
			FVector3 vec = *(FVector3*)(&vData[i]);
			level.LevelGeometry.Root->BoundingBox.Enclose(vec);
		}


		auto brush = new StaticGeometry(scene->CreateStaticRigidBody(FTransform()));
		brush->PhysicsBody.SetShape(PhysShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), scene->CookTriangleMeshGeometry(triangleMeshDesc)));

		brush->VertexBuffer = allocator->CreateVertexBuffer(geomHeader.vLength, geomHeader.elementSize, EFRIAccess::None, EFRIUsage::Default, vertexDesc);
		brush->IndexBuffer = allocator->CreateIndexBuffer(geomHeader.iLength, EFRIAccess::None, EFRIUsage::Default, indexDesc);
		//allocator->AttachInputLayout(brush->VertexBuffer, vertexDeclaration);

		brush->Material = Materials[geomHeader.MaterialID];

		level.LevelGeometry.Leafs.Add(brush);
	}

	cmdList.CloseAndExecute();

	return level;
}