#include "AssetManager.h"

template<typename modelType>
FLRESULT AssetManager::LoadModelFromFile(STRING path, _Out_ modelType* modelType)
{
	return FLRESULT::SUCCESS;
}

template<>
FLRESULT AssetManager::LoadModelFromFile<StaticModel>(STRING path, StaticModel* model)
{


	// ----------------------------------
	//			HEADER
	//-----------------------------------

	FLFileStream flStream(path);


	char file_signature[4];
	flStream.ReadArray(file_signature);


	if (!(file_signature[0] == 'F' &&
		file_signature[1] == 'L' &&
		file_signature[2] == '3' &&
		file_signature[3] == 'D'))
	{
		return FLRESULT::FAIL;
	}

	BYTE file_version[4];
	flStream.ReadArray(file_version);

	if (!(file_version[0] == 1 &&
		file_version[1] == 0 &&
		file_version[2] == 0 &&
		file_version[3] == 2))
	{
		return FLRESULT::FAIL;
	}


	ModelTypeEnum model_type = (ModelTypeEnum)flStream.Read<int>();




	BYTE file_checksum[16];
	flStream.ReadArray(file_checksum);


	// ----------------------------------
	//			MESH TABLE
	//-----------------------------------

	int meshCount = flStream.Read<int>();

	if (meshCount < 1)
		return FLRESULT::FAIL;


	/*int* meshDataLengths = Memory::Create<int>(meshCount);
	flStream.ReadArray(meshDataLengths, meshCount);
	*/

	// ----------------------------------
	//			MESHES
	//-----------------------------------
	
	

	for (int i = 0; i < meshCount; i++)
	{
		model->meshCollection.push_back(StaticModelMesh());


		FMatrix4 localTransform(0);
		localTransform = flStream.Read<FMatrix4>();


		// Material Info
		int materialNameLength = flStream.Read<int>();

		char* materialName = Memory::Create<char>(materialNameLength+1);
		flStream.ReadArray(materialName, materialNameLength);

		materialName[materialNameLength] = '\0';

		model->meshCollection[i].mMeshMaterial = new BakedMaterial();
		LoadMaterialFromFile(STRING(materialName), model->meshCollection[i].mMeshMaterial);


		// Geometry Info

		unsigned long long vDataLength = flStream.Read<unsigned long long>();
		unsigned long long iDataLength = flStream.Read<unsigned long long>();
		
		if (vDataLength == 0 || iDataLength == 0)
			continue;


		StaticModelVertex*	 vData = Memory::Create<StaticModelVertex>(vDataLength);
		unsigned int*		 iData = Memory::Create<unsigned int>(iDataLength);

		flStream.ReadArray(vData, vDataLength / sizeof(StaticModelVertex));
		flStream.ReadArray(iData, iDataLength / sizeof(unsigned int));



		model->meshCollection[i].LocalTransform = localTransform;
		model->meshCollection[i].mVertexBuffer = new VertexBuffer(StaticModelVertex::Elements);
		model->meshCollection[i].mVertexBuffer->SetIndexedData(vData, iData, vDataLength / sizeof(StaticModelVertex), iDataLength / sizeof(unsigned int));

		Shader* modelShaders[2] =
		{
			FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao_geom.vert", ShaderType::VERTEX),
			FLSLCompilerService::CompileShaderFromSourceFile(".\\shaders\\ssao_geom.frag", ShaderType::FRAGMENT)
		};


		model->meshCollection[i].pRenderingProgram = new Program(modelShaders); 
	}
}


// --- MATERIAL LOADING


template<typename materialType>
FLRESULT AssetManager::LoadMaterialFromFile(STRING path, _Out_ materialType* m)
{

}

template<>
FLRESULT AssetManager::LoadMaterialFromFile<BakedMaterial>(STRING path, _Out_ BakedMaterial* material)
{

	// ----------------------------------
	//			HEADER
	//-----------------------------------

	FLFileStream flStream(path);


	char file_signature[4];
	flStream.ReadArray(file_signature);


	if (!(file_signature[0] == 'F' &&
		file_signature[1] == 'L' &&
		file_signature[2] == 'M' &&
		file_signature[3] == 'T'))
	{
		return FLRESULT::FAIL;
	}

	BYTE file_version[4];
	flStream.ReadArray(file_version);

	if (!(file_version[0] == 1 &&
		file_version[1] == 0 &&
		file_version[2] == 0 &&
		file_version[3] == 0))
	{
		return FLRESULT::FAIL;
	}

	int plchldr = flStream.Read<int>();

	BYTE file_checksum[16];
	flStream.ReadArray(file_checksum);


	// ----------------------------------
	//			MAP DATA
	//-----------------------------------

	int mapCount = flStream.Read<int>();

	for (int i = 0; i < mapCount; i++)
	{
		int mapNameLength = flStream.Read<int>();
		char* mapName = Memory::Create<char>(mapNameLength + 1);
		flStream.ReadArray(mapName, mapNameLength);
		mapName[mapNameLength] = '\0';

		int mapWidth = flStream.Read<int>();
		int mapHeight = flStream.Read<int>();
		int channels = flStream.Read<int>();

		BYTE* pixelData = Memory::Create<BYTE>(mapWidth * mapHeight * channels);
		flStream.ReadArray(pixelData, mapWidth * mapHeight * channels);

		Texture* mapTexture = new Texture(mapWidth, mapHeight, GL_RGBA32F, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, false);
		mapTexture->SetData(pixelData);

		mapTexture->SetFilteringMode(TextureFiltering::BILINEAR);
		mapTexture->SetWrappingMode(TextureWrapping::REPEAT);

		material->SetMap(STRING(mapName), mapTexture);
	}

}

bool AssetManager::ValidateSignature(SignatureType s)
{
	if (s == SignatureType::Model)
	{

	}

	return true;
}