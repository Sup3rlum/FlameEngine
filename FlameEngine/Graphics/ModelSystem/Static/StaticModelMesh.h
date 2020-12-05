#pragma once

#include "../../../dll/nchfx.h"
#include "../Base/ModelBase.h"


EXPORT(class, StaticModelMesh) : public ModelMeshBase
{
public:
	void Update();
	void Render(Scene* scene, FMatrix4 transform);
};

