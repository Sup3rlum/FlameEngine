#pragma once


#include "../AssetManager.h"

#include "Core/Engine/GameSystem/Mesh.h" 
#include "Core/Engine/FlameRI/FRI.h"

template<>
EXPORT(struct, TContentSerializer<RiggedMesh>)
{
private:

	FRIInputLayout* vertexDeclaration;
	FRIContext* renderContext;

public:
	TContentSerializer(IAssetManager* manager);
	RiggedMesh Serialize(Stream& fileStream);
};

