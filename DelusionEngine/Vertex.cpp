#include "Vertex.h"


vector<VertexElements> VertexTexture::Elements =
{
	VertexElements(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0),
	VertexElements(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)))
};

vector<VertexElements> VertexNormalTexture::Elements =
{
	VertexElements(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0),
	VertexElements(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))),
	VertexElements(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))),
};

vector<VertexElements> VertexNormalTextureAmbience::Elements =
{
	VertexElements(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0),
	VertexElements(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))),
	VertexElements(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))),
	VertexElements(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float))),
};


vector<VertexElements> VertexColor::Elements =
{
	VertexElements(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0),
	VertexElements(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)))
};