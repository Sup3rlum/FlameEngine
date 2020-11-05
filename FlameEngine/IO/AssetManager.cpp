#include "AssetManager.h"


void AssetManager::LoadModel(STRING path, Model* m)
{

	vector<BYTE> bytedata = File::ReadAllBytes(path);

	int currentPosition = 0;

	if (bytedata.size() < 20)
	{
		return;
	}


	if (!(bytedata[0] == 'F' &&
		bytedata[1] == 'L' &&
		bytedata[2] == '3' &&
		bytedata[3] == 'D'))
	{
		return;
	}

	if (!(bytedata[4] == 1 &&
		bytedata[5] == 0 &&
		bytedata[6] == 0 &&
		bytedata[7] == 0))
	{
		return;
	}

	currentPosition += 8;

	int _static = Memory::ToInt(&bytedata[currentPosition]);
	//size_t iLength = Memory::ToInt(&bytedata[12]); TODO Checksum

	float* modelTransform = (float*)malloc(16 * sizeof(float));

	currentPosition += 20;

	memcpy(modelTransform, &bytedata[currentPosition], 64);


	// Material
	currentPosition += 64;
	int _materialPathLength = Memory::ToInt(&bytedata[currentPosition]);
	currentPosition += 4;
	STRING _materialPath = Memory::ToString(&bytedata[currentPosition], _materialPathLength);

	m->_material = new Material();
	m->_material->colorMap = new Texture(_materialPath);


	// Vert Shader
	currentPosition += _materialPathLength;
	int _vertShaderLength = Memory::ToInt(&bytedata[currentPosition]);
	currentPosition += 4;
	STRING _vertShader = Memory::ToString(&bytedata[currentPosition], _vertShaderLength);

	// Frag Shader
	currentPosition += _vertShaderLength;
	int _fragShaderLength = Memory::ToInt(&bytedata[currentPosition]);
	currentPosition += 4;
	STRING _fragShader = Memory::ToString(&bytedata[currentPosition], _fragShaderLength);


	// Data
	currentPosition += _fragShaderLength;
	_UNS_ FL_INT64 vLength = Memory::ToULLong(&bytedata[currentPosition]);
	currentPosition += 8;
	_UNS_ FL_INT64 iLength = Memory::ToULLong(&bytedata[currentPosition]);


	VertexNormalTexture*	vData =	(VertexNormalTexture*)malloc(vLength);
	_UNS_ FL_INT32 *		iData =	(_UNS_ FL_INT32*)malloc(iLength);

	if (vData == NULL || iData == NULL)
		return;

	currentPosition += 8;
	memcpy(vData, &bytedata[currentPosition], vLength);
	currentPosition += vLength;
	memcpy(iData, &bytedata[currentPosition], iLength);


	m->_vbo = VertexBuffer(VertexNormalTexture::Elements);
	m->_vbo.SetIndexedData<VertexNormalTexture>(vData, iData, vLength / 32, iLength / 4);

	m->_shader = Shader::FromSource(".\\shaders\\animated_model_vert.glsl", ".\\shaders\\animated_model_frag.glsl");
	m->_depthShader = Shader::FromSource(".\\shaders\\depth_vert.glsl", ".\\shaders\\depth_frag.glsl");
}


void AssetManager::LoadMaterial(STRING path, _Out_ Material* m)
{

}

bool AssetManager::ValidateSignature(SignatureType s)
{
	if (s == SignatureType::Model)
	{

	}

	return true;
}