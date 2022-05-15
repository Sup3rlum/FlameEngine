#pragma once


#include "../AssetManager.h"

#include "Core/Engine/GameSystem/SkinnedMesh.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<SkinnedMesh>)
{
private:

	FRIVertexDeclaration* vertexDeclaration;
	FRIContext* renderContext;

public:
	TContentSerializer(IAssetManager* manager);
	SkinnedMesh Serialize(Stream& fileStream);
};

