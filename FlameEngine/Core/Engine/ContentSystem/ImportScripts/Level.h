#pragma once

#include "../AssetManager.h"

#include "Core/Engine/GameSystem/Environment/Level.h" 
#include "Core/Engine/FlameRI/FRI.h"

class Scene;

template<>
EXPORT(struct, TContentSerializer<Level>)
{
private:
	FRIVertexDeclaration* vertexDeclaration;
	FRIContext* renderContext;
	Scene* scene;

	IAssetManager* Content;

public:
	TContentSerializer(IAssetManager * manager, Scene * scene);
	Level Serialize(Stream& fileStream);
};
