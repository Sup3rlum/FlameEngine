#pragma once

#include "../Graphics/ModelSystem/Base/ModelBase.h"
#include "../Graphics/ModelSystem/Animated/AnimatedModel.h"
#include "../Graphics/ModelSystem/Fracture/FractureModel.h"
#include "../Graphics/ModelSystem/Static/StaticModel.h"


#include "../Graphics/MaterialSystem/Base/MaterialBase.h"
#include "../Graphics/MaterialSystem/Baked/BakedMaterial.h"
#include "../Graphics/MaterialSystem/Animated/AnimatedMaterial.h"


#include "../Graphics/ShaderDefinitions/CompilationServices/FLSLCompilerService.h"
#include "../Graphics/Common/Vertex.h"
#include "../FileSystem/File.h"
#include "../flameRT/Memory.h"
#include "../Framework/Common/FLFileStream.h"
#include "../util/result.h"

using namespace std;


EXPORT_ENUM SignatureType
{
	Model,
	Material
};


EXPORT_ENUM ModelTypeEnum
{
	STATIC = 1,
	ANIMATED = 2,
	FRACTURE = 3
};



EXPORT(class,  AssetManager)
{
private:
	static bool ValidateSignature(SignatureType s);
public:


	// -- Model Loading

	template<typename modelType>
	static FLRESULT LoadModelFromFile(STRING path, _Out_ modelType* m);

	template<>
	static FLRESULT LoadModelFromFile<AnimatedModel>(STRING path,	_Out_ AnimatedModel * modelType);
	template<>
	static FLRESULT LoadModelFromFile<StaticModel>(STRING path,		_Out_ StaticModel * modelType);
	template<>
	static FLRESULT LoadModelFromFile<FractureModel>(STRING path,	_Out_ FractureModel * modelType);


	// -- Material Loading

	template<typename materialType>
	static FLRESULT LoadMaterialFromFile(STRING path, _Out_ materialType * m);

	template<>
	static FLRESULT LoadMaterialFromFile<BakedMaterial>(STRING path, _Out_ BakedMaterial * m);
	template<>
	static FLRESULT LoadMaterialFromFile<AnimatedMaterial>(STRING path, _Out_ AnimatedMaterial* m);
};

