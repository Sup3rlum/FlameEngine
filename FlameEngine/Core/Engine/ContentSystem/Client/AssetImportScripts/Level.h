#pragma once


#include "../LocalContentService.h"

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

public:
	TContentSerializer(FRIContext * friContext, Scene* scene);

	Level Serialize(IOFileStream & fileStream);


};


struct FLevelFileDescriptor : public FLocalContentFileDescriptionBase
{
	FLevelFileDescriptor() : FLocalContentFileDescriptionBase("FLEN", ELocalContentFileStorage::BINARY) {}
};