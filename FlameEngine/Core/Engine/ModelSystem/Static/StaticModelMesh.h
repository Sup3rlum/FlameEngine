#pragma once

#include "../../../dll/nchfx.h"
#include "../Base/ModelBase.h"


EXPORT(class, StaticModelMesh) : public ModelMeshBase
{
public:
	void Update();
	void Render(Program* pRenderingProgram, Scene* scene, FMatrix4 transform);
};

