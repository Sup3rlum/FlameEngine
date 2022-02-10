#pragma once

#include "../GameSystem/Common/Scene.h"
#include "../GameSystem/Mesh.h"
#include "../GameSystem/SkinnedMesh.h"
#include "../GameSystem/Material/MaterialComponent.h"


struct FRenderSystemGeom		: public FEntityComponentSystem<Mesh, Material, FTransform>		{ void Update(Entity e, Mesh& m, Material& mc, FTransform& f) {} };
struct FRenderSystemSkinnedGeom : public FEntityComponentSystem<SkinnedMesh, Material, FTransform>	{ void Update(Entity e, SkinnedMesh& m, Material& mc, FTransform& f) {} };

struct FRenderSystemSM			: public FEntityComponentSystem<Mesh, FTransform>			{ void Update(Entity e, Mesh& m, FTransform& f) {} };
struct FRenderSystemSkinnedSM	: public FEntityComponentSystem<SkinnedMesh, FTransform>	{ void Update(Entity e, SkinnedMesh& m, FTransform& f) {} };


struct FRenderDLight : public FEntityComponentSystem<DirectionalLight>	{ void Update(Entity, DirectionalLight&) {} };
struct FRenderPLight : public FEntityComponentSystem<PointLight>		{ void Update(Entity, PointLight&) {} };
struct FRenderSLight : public FEntityComponentSystem<SpotLight>			{ void Update(Entity, SpotLight&) {} };
