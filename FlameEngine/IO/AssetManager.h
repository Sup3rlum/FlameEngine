#pragma once

#include "../Scene/Model.h"
#include "../Scene/Material.h"
#include "../Graphics/Vertex.h"
#include "File.h"
#include "Memory.h"

using namespace std;

EXPORT_CLASS AssetManager
{
public:
	static void LoadModel(STRING path, Model* m);
};

