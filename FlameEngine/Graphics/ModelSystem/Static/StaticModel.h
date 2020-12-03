#pragma once

#include "../../../dll/nchfx.h"
#include "../Base/ModelBase.h"
#include "../Static/StaticModelMesh.h"


EXPORT(class, StaticModel) : public ModelBase
{
public:
	void Render(Scene* scene);
	void Update();

	std::vector<StaticModelMesh> meshCollection;
};

