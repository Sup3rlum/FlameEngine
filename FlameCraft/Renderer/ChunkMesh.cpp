#include "ChunkMesh.h"
#include <FlameEngine/Core/Engine/GameSystem/Common/Scene.h>

#include <iostream>




FVector3 offsets[6][4] =
{
	{ // UP +Y
		FVector3(1,1,0),
		FVector3(0,1,0),
		FVector3(0,1,1),
		FVector3(1,1,1)
	},
	{ // DOWN -Y
		FVector3(0,0,0),
		FVector3(1,0,0),
		FVector3(1,0,1),
		FVector3(0,0,1)
	},
	{ // LEFT -X
		FVector3(0,1,1),
		FVector3(0,1,0),
		FVector3(0,0,0),
		FVector3(0,0,1)
	},
	{ // RIGHT +X
		FVector3(1,1,0),
		FVector3(1,1,1),
		FVector3(1,0,1),
		FVector3(1,0,0)
	},
	{ // FRONT +Z
		FVector3(1,1,1),
		FVector3(0,1,1),
		FVector3(0,0,1),
		FVector3(1,0,1)
	},
	{ // BACK -Z
		FVector3(0,1,0),
		FVector3(1,1,0),
		FVector3(1,0,0),
		FVector3(0,0,0)
	}
};


FVector3 Normals[6][3] =
{
	{
		FVector3(0, 1, 0),
		FVector3(0, 0, 1),
		FVector3(1, 0, 0)
	},
	{
		FVector3(0, -1, 0),
		FVector3(0, 0, -1),
		FVector3(1, 0, 0)
	},
	{
		FVector3(-1, 0, 0),
		FVector3(0, 1, 0),
		FVector3(0, 0, 1)
	},
	{
		FVector3(1, 0, 0),
		FVector3(0, 1, 0),
		FVector3(0, 0, -1)
	},
	{ 
		FVector3(0, 0, 1),
		FVector3(1, 0, 0),
		FVector3(0, 1, 0),
	},
	{
		FVector3(0, 0, -1),
		FVector3(1, 0, 0),
		FVector3(0, 1, 0)
	}
};

FVector2 uvs[4] =
{
	FVector2(0,0),
	FVector2(1,0),
	FVector2(1,1),
	FVector2(0,1)
};



int faceIDs[11][6]
{
	{0, 0, 0, 0, 0, 0},	// AIR
	{1, 1, 1, 1, 1, 1},	// STONE
	{3, 4, 2, 2, 2, 2}, // GRASS_BLOCK
	{4, 4, 4, 4, 4, 4}, // DIRT
	{5, 5, 5, 5, 5, 5}, // COBBLE
	{6, 6, 6, 6, 6, 6}, // PLANKS
	{7, 7, 7, 7, 7, 7}, // SAND
	{9, 9, 8, 8, 8, 8},	// OAK LOG
	{10, 10, 10, 10, 10, 10},	// LEAVES
	{11, 11, 11, 11, 11, 11},	// WATER
	{12, 12, 12, 12, 12, 12}	// GRASS
};

#define IS_TRANSPARENT(b) (b.ID == BLOCK_AIR || b.ID == BLOCK_LEAVES || b.ID == BLOCK_WATER || b.ID == BLOCK_GRASS) && (b.ID != block.ID)



Block GetBlockGlobal(FHashMap<IVector2, Chunk*, ChunkIDHasher>& data, IVector3 pos)
{
	IVector2 chunkCoord = World::GlobalToChunkCoord(pos);
	bool contains = data.Contains(chunkCoord);

	if (!contains)
		return Block(0);

	auto chunkPtr = data[chunkCoord];

	IVector3 blockPosChunk = World::GlobalToChunk(pos);
	return chunkPtr->GetBlock(blockPosChunk);
}


void ChunkMesh::WriteBlockFace(Block block, BlockFace face, MeshDesc& mesh, FVector3 blockPos)
{
	FVertex fv;

	for (int i = 0; i < 4; i++)
	{
		FVector3 pos = blockPos + offsets[face][i];
		fv.Pos = pos;
		fv.Normal = Normals[face][0];
		fv.Tangent = Normals[face][1];
		fv.Bitangent = Normals[face][2];
		fv.TexCoord = FVector3(uvs[i], faceIDs[block.ID][face]);

		mesh.VerticesData.Write(fv);
	}

	int firstIndex = 0;
	if (mesh.IndexData.Length() > 0)
		firstIndex = mesh.IndexData.Top() + 1;

	mesh.IndexData.Write(firstIndex);
	mesh.IndexData.Write(firstIndex + 1);
	mesh.IndexData.Write(firstIndex + 2);

	mesh.IndexData.Write(firstIndex);
	mesh.IndexData.Write(firstIndex + 2);
	mesh.IndexData.Write(firstIndex + 3);
}

void ChunkMesh::WriteBlock(
	Block block,
	const IVector3& blockPosWorld,
	const IVector3& blockPosChunk,
	FHashMap<IVector2, Chunk*, ChunkIDHasher>& chunkData,
	Chunk* chunk,
	MeshDesc& meshDesc
	)
{
	Block blockUp, blockDown, blockLeft, blockRight, blockFront, blockBack;

	int edgeIndex = 
		(blockPosChunk.x % 15) *
		(blockPosChunk.y % 15) * 
		(blockPosChunk.z % 15);

	if (!edgeIndex)
	{
		blockUp = GetBlockGlobal(chunkData, blockPosWorld + IVector3(0, 1, 0));
		blockDown = GetBlockGlobal(chunkData, blockPosWorld - IVector3(0, 1, 0));
		blockLeft = GetBlockGlobal(chunkData, blockPosWorld - IVector3(1, 0, 0));
		blockRight = GetBlockGlobal(chunkData, blockPosWorld + IVector3(1, 0, 0));
		blockFront = GetBlockGlobal(chunkData, blockPosWorld + IVector3(0, 0, 1));
		blockBack = GetBlockGlobal(chunkData, blockPosWorld - IVector3(0, 0, 1));
	}
	else
	{
		blockUp = chunk->GetBlock(blockPosChunk + IVector3(0, 1, 0));
		blockDown = chunk->GetBlock(blockPosChunk - IVector3(0, 1, 0));
		blockLeft = chunk->GetBlock(blockPosChunk - IVector3(1, 0, 0));
		blockRight = chunk->GetBlock(blockPosChunk + IVector3(1, 0, 0));
		blockFront = chunk->GetBlock(blockPosChunk + IVector3(0, 0, 1));
		blockBack = chunk->GetBlock(blockPosChunk - IVector3(0, 0, 1));
	}


	if (IS_TRANSPARENT(blockUp))	WriteBlockFace(block, UP, meshDesc, blockPosWorld);
	if (IS_TRANSPARENT(blockDown))	WriteBlockFace(block, DOWN, meshDesc, blockPosWorld);
	if (IS_TRANSPARENT(blockLeft))	WriteBlockFace(block, LEFT, meshDesc, blockPosWorld);
	if (IS_TRANSPARENT(blockRight)) WriteBlockFace(block, RIGHT, meshDesc, blockPosWorld);
	if (IS_TRANSPARENT(blockFront)) WriteBlockFace(block, FRONT, meshDesc, blockPosWorld);
	if (IS_TRANSPARENT(blockBack))	WriteBlockFace(block, BACK, meshDesc, blockPosWorld);

}

void ChunkMesh::WriteTessPatchFace(Block block, float rotZ, float rotY, MeshDesc& mesh, FVector3 blockPos)
{
	FVertex fv;

	FVector3 vertices[4];
	FVector3 normal;

	vertices[0] = FVector3(-cosf(rotY), 1, -sinf(rotY));
	vertices[1] = FVector3(cosf(rotY), 1, sinf(rotY));
	vertices[2] = FVector3(cosf(rotY), 0, sinf(rotY));
	vertices[3] = FVector3(-cosf(rotY), 0, -sinf(rotY));


	normal = FVector3(1, 0, 0);

	for (int i = 0; i < 4; i++)
	{
		fv.Pos = blockPos + vertices[i] + FVector3(0.5f, 0.0, 0.5f);
		fv.Normal = normal;
		fv.Tangent = normal;
		fv.Bitangent = normal;;
		fv.TexCoord = FVector3(uvs[i], faceIDs[block.ID][0]);

		mesh.VerticesData.Write(fv);
	}

	int firstIndex = 0;
	if (mesh.IndexData.Length() > 0)
		firstIndex = mesh.IndexData.Top() + 1;

	mesh.IndexData.Write(firstIndex);
	mesh.IndexData.Write(firstIndex + 1);
	mesh.IndexData.Write(firstIndex + 2);

	mesh.IndexData.Write(firstIndex);
	mesh.IndexData.Write(firstIndex + 2);
	mesh.IndexData.Write(firstIndex + 3);
}

int grassDetail = 2;

void ChunkMesh::WriteTessPatch(
	Block block,
	const IVector3& blockPosWorld,
	const IVector3& blockPosChunk,
	FHashMap<IVector2, Chunk*, ChunkIDHasher>& chunkData,
	Chunk* chunk,
	MeshDesc& meshDesc
)
{
	for (int i = 0; i < 2; i++)
	{
		float rotY = PI * (float)(rand() % 90) / 180.0f;
		float rotZ = PI * (float)(rand() % 90) / 180.0f;

		WriteTessPatchFace(block, rotZ, rotY, meshDesc, blockPosWorld);
		WriteTessPatchFace(block, rotZ, rotY + PI, meshDesc, blockPosWorld);
	}
}


ChunkMeshDescription ChunkMesh::RegenerateGeometry(Chunk* chunk)
{
	ChunkMeshDescription desc;

	auto chunkData = world->ChunkData;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_DEPTH; z++)
			{
				IVector3 blockPosChunk(x, y, z);
				IVector3 blockPosWorld = World::ChunkToGlobal(position, blockPosChunk);

				auto block = chunk->GetBlock(blockPosChunk);

				if (block.ID == BLOCK_AIR)
					continue;

				if (block.ID == BLOCK_WATER)
				{
					WriteBlock(
						block, 
						blockPosWorld, 
						blockPosChunk, 
						chunkData, 
						chunk, 
						desc.waterGeometry);
				}
				else if (block.ID == BLOCK_GRASS)
				{
					WriteTessPatch(
						block,
						blockPosWorld,
						blockPosChunk,
						chunkData,
						chunk,
						desc.grassGeometry);
				}
				else
				{
					WriteBlock(
						block,
						blockPosWorld,
						blockPosChunk,
						chunkData,
						chunk,
						desc.staticGeometry);
				}
			}
		}
	}

	PhysicsTriangleMeshDesc triangleMeshDesc;
	triangleMeshDesc.DataStride = sizeof(FVertex);
	triangleMeshDesc.VertexData = desc.staticGeometry.VerticesData.GetFRIDescriptor();
	triangleMeshDesc.IndexData = desc.staticGeometry.IndexData.GetFRIDescriptor();

	desc.PhysicsGeom = world->scene->CookTriangleMeshGeometry(triangleMeshDesc);
	desc.mesh = this;

	return desc;
}

void ChunkMesh::StageGeometry(FRICommandList& cmdList, ChunkMeshDescription desc,  FRIVertexDeclaration* vDecl)
{

	rigidBody.RemoveShapes();
	rigidBody.SetShape(PhysShape(PhysicsMaterial(0.8f, 0.8f, 0.1f), desc.PhysicsGeom));


	StageGeometryMesh(cmdList, staticGeometry, desc.staticGeometry, vDecl);
	StageGeometryMesh(cmdList, waterGeometry, desc.waterGeometry, vDecl);
	StageGeometryMesh(cmdList, grassGeometry, desc.grassGeometry, vDecl);

}

void ChunkMesh::StageGeometryMesh(FRICommandList& cmdList, GeometryMesh& mesh, MeshDesc& meshdesc, FRIVertexDeclaration* vDecl)
{
	if (!meshdesc.IsValid())
		return;

	mesh.vertexBuffer = cmdList.GetDynamic()->CreateVertexBuffer(meshdesc.VerticesData.Length(), 0, meshdesc.VerticesData.GetFRIDescriptor());
	mesh.indexBuffer = cmdList.GetDynamic()->CreateIndexBuffer(meshdesc.IndexData.Length(), 0, meshdesc.IndexData.GetFRIDescriptor());

	cmdList.GetDynamic()->AttachVertexDeclaration(mesh.vertexBuffer, vDecl);

	meshdesc.Free();
}

ChunkMesh::ChunkMesh(World* _world, IVector2 position) :
	rigidBody(_world->scene->CreateStaticRigidBody(FTransform())),
	world(_world),
	position(position)
{

}

void ChunkMesh::RenderStatic(FRICommandList& cmdList)
{
	if (!staticGeometry.IsValid())
		return;

	cmdList.SetGeometrySource(staticGeometry.vertexBuffer);
	cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, staticGeometry.indexBuffer->IndexCount, EFRIIndexType::UInt32, staticGeometry.indexBuffer);
}

void ChunkMesh::RenderWater(FRICommandList& cmdList)
{
	if (!waterGeometry.IsValid())
		return;

	cmdList.SetGeometrySource(waterGeometry.vertexBuffer);
	cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, waterGeometry.indexBuffer->IndexCount, EFRIIndexType::UInt32, waterGeometry.indexBuffer);
}


void ChunkMesh::RenderGrass(FRICommandList& cmdList)
{
	if (!grassGeometry.IsValid())
		return;

	cmdList.SetGeometrySource(grassGeometry.vertexBuffer);
	cmdList.DrawPrimitivesIndexed(EFRIPrimitiveType::Triangles, grassGeometry.indexBuffer->IndexCount, EFRIIndexType::UInt32, grassGeometry.indexBuffer);
}


void ChunkMesh::DeleteGeometry()
{
	staticGeometry.Delete();
	waterGeometry.Delete();
	grassGeometry.Delete();

}