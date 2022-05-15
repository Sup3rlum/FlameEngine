#pragma once

#include <FlameEngine/Core/Engine/GameSystem/RenderObject.h>
#include <FlameEngine/Core/Framework/IO/Stream.h>
#include <FlameEngine/Core/Engine/GameSystem/Physics/PhysicsComponent.h>

#include "../World/World.h"
#include "GeometryStream.h"

struct FVertex
{
	FVector3 Pos;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 Bitangent;
	FVector3 TexCoord;
};

enum BlockFace
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FRONT,
	BACK
};

struct MeshDesc
{
	FMemoryBlock<FVertex> VerticesData;
	FMemoryBlock<int> IndexData;

	bool IsValid()
	{
		return (VerticesData.ByteSize() && IndexData.ByteSize());
	}

	void Free()
	{
		VerticesData.Delete();
		IndexData.Delete();
	}

};

struct ChunkMeshDescription
{
	class ChunkMesh* mesh;
	
	MeshDesc staticGeometry;
	MeshDesc waterGeometry;
	MeshDesc grassGeometry;

	bool loaded = false;
	TriangleMeshGeometry PhysicsGeom;
};


struct GeometryMesh
{
	FRIVertexBuffer* vertexBuffer = 0;
	FRIIndexBuffer* indexBuffer = 0;

	void Delete()
	{
		if (vertexBuffer)
		{
			delete vertexBuffer;
			vertexBuffer = 0;
		}
		if (indexBuffer)
		{
			delete indexBuffer;
			indexBuffer = 0;
		}
	}

	bool IsValid()
	{
		return vertexBuffer && indexBuffer;
	}
};



class ChunkMesh
{
public:
	GeometryMesh staticGeometry;
	GeometryMesh waterGeometry;
	GeometryMesh grassGeometry;


	StaticRigidBody rigidBody;

	IVector2 position;
	World* world;


	ChunkMesh(World* world, IVector2 position);
	void DeleteGeometry();

	void RenderStatic(FRICommandList& cmdList);
	void RenderWater(FRICommandList& cmdList);
	void RenderGrass(FRICommandList& cmdList);
	ChunkMeshDescription RegenerateGeometry(Chunk* chunk);
	void StageGeometry(FRICommandList& cmdList, ChunkMeshDescription desc, FRIVertexDeclaration* vDecl);
	void StageGeometryMesh(FRICommandList& cmdList, GeometryMesh& mesh, MeshDesc& meshdesc, FRIVertexDeclaration* vDecl);


	FORCEINLINE void WriteBlock(
		Block block,
		const IVector3& blockPosWorld,
		const IVector3& blockPosChunk,
		FHashMap<IVector2, Chunk*, ChunkIDHasher>& chunkData,
		Chunk* chunk,
		MeshDesc& meshDesc
	);
	FORCEINLINE void WriteBlockFace(Block block, BlockFace face, MeshDesc& mesh, FVector3 blockPos);

	FORCEINLINE void WriteTessPatch(
		Block block,
		const IVector3& blockPosWorld,
		const IVector3& blockPosChunk,
		FHashMap<IVector2, Chunk*, ChunkIDHasher>& chunkData,
		Chunk* chunk,
		MeshDesc& meshDesc
	);
	FORCEINLINE void WriteTessPatchFace(Block block, float rotX, float rotY, MeshDesc& mesh, FVector3 blockPos);

};
