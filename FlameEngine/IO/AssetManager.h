#pragma once

#include "../Scene/Model.h"
#include "../Scene/Material.h"
#include "../Graphics/Vertex.h"
#include "File.h"
#include "Memory.h"

using namespace std;


EXPORT_ENUM SignatureType
{
	Model,
	Material
};


EXPORT_CLASS AssetManager
{
private:
	static bool ValidateSignature(SignatureType s);
public:
	static void LoadModel(STRING path, _Out_ Model* m);
	static void LoadMaterial(STRING path, _Out_ Material* m);
};

