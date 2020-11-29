#pragma once

#include "../Graphics/ModelSystem/ModelBase.h"
#include "../Graphics/Common/Material.h"
#include "../Graphics/Common/Vertex.h"
#include "../FileSystem/File.h"
#include "../flameRT/Memory.h"

using namespace std;


EXPORT_ENUM SignatureType
{
	Model,
	Material
};


EXPORT(class,  AssetManager)
{
private:
	static bool ValidateSignature(SignatureType s);
public:
	static void LoadModel(STRING path, _Out_ ModelBase* m);
	static void LoadMaterial(STRING path, _Out_ Material* m);
};

