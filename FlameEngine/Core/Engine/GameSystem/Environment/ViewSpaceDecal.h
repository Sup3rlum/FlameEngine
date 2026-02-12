#pragma once

#include "Core/Engine/FlameRI/FRI.h"
#include "../Material/Material.h"
#include "../../Renderer/Common/RenderUtil.h"
#include "../Mesh.h" 

EXPORT(class, ViewSpaceDecal)
{
public:

	ViewSpaceDecal() {}
	void AddToRenderList(FRICommandList& cmdList) const;

	Mesh mesh;
	Material material;
};

