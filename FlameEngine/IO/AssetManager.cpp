#include "AssetManager.h"


void AssetManager::LoadModel(STRING path, Model* m)
{

	vector<BYTE> bytedata = File::ReadAllBytes(path);


	if (bytedata.size() < 20)
	{
		return;
	}


	if (!(bytedata[0] == 'D' &&
		bytedata[1] == 'E' &&
		bytedata[2] == 'M' &&
		bytedata[3] == 'F'))
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

	size_t vLength = Memory::ToLLong(&bytedata[8]);
	size_t iLength = Memory::ToLLong(&bytedata[16]);

	VertexNormalTexture* vData =	(VertexNormalTexture*)malloc(vLength);
	_UNS_ FL_INT32 * iData =				(_UNS_ FL_INT32*)malloc(iLength);

	if (vData == NULL || iData == NULL)
		return;

	memcpy(vData, &bytedata[24], vLength);
	memcpy(iData, &bytedata[24 + vLength], iLength);


	ModelMesh mesh(0);

	m->children.push_back(ModelMesh(0));

	m->children[0]._vbo = VertexBuffer(VertexNormalTexture::Elements);
	m->children[0]._vbo.SetIndexedData<VertexNormalTexture>(vData, iData, vLength / 32, iLength / 4);

	m->children[0]._shader = new Shader(".\\shaders\\vert.glsl", ".\\shaders\\frag.glsl");

	m->children[0]._material = new Material();
	m->children[0]._material->colorMap = new Texture(".\\textures\\dirt.jpg");

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