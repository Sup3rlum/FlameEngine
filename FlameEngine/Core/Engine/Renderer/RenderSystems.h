#pragma once

#include "../GameSystem/Common/Scene.h"
#include "../GameSystem/MeshComponent.h"
#include "../GameSystem/SkinnedMeshComponent.h"
#include "../GameSystem/Material/MaterialComponent.h"


struct FRenderSystemGeom		: public FEntityComponentSystem<MeshComponent, MaterialComponent, FTransform>			{ void Update(Entity e, MeshComponent& m, MaterialComponent& mc, FTransform& f) {} };
struct FRenderSystemSkinnedGeom : public FEntityComponentSystem<SkinnedMeshComponent, MaterialComponent, FTransform>	{ void Update(Entity e, SkinnedMeshComponent& m, MaterialComponent& mc, FTransform& f) {} };

struct FRenderSystemSM			: public FEntityComponentSystem<MeshComponent, FTransform>			{ void Update(Entity e, MeshComponent& m, FTransform& f) {} };
struct FRenderSystemSkinnedSM	: public FEntityComponentSystem<SkinnedMeshComponent, FTransform>	{ void Update(Entity e, SkinnedMeshComponent& m, FTransform& f) {} };