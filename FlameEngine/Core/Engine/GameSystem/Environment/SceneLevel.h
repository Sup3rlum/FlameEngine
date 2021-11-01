#pragma once


#include "Core/Engine/FlameRI/FRI.h"
#include "../MeshComponent.h"
#include "../Material/MaterialComponent.h"

struct LevelBrush
{
	Mesh BrushMesh;
	Material BrushMaterial;
};



struct SceneLevel
{
	FArray<LevelBrush*> Brushes;


	~SceneLevel()
	{
		for (auto b : Brushes)
		{
			delete b;
		}
	}
};