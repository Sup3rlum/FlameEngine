#pragma once

#include "nchfx.h"
#include "VertexElements.h"

using namespace glm;
using namespace std;

EXPORT_STRUCT VertexNormalTexture
{
	Vector3 _position;
	Vector3 _normal;
	Vector2 _texCoord;

	VertexNormalTexture(Vector3 pos, Vector3 norm, Vector2 tex)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
	}

	static vector<VertexElements> Elements;
};

EXPORT_STRUCT VertexTexture
{
	Vector3 _position;
	Vector2 _texCoord;

	VertexTexture(Vector3 pos, Vector2 tex)
	{
		_position = pos;
		_texCoord = tex;
	}

	static vector<VertexElements> Elements;
};

EXPORT_STRUCT VertexColor
{
	Vector3 _position;
	Vector3 _color;

	VertexColor(Vector3 pos, Vector3 col)
	{
		_position = pos;
		_color = col;
	}

	static vector<VertexElements> Elements;
};

EXPORT_STRUCT VertexNormalTextureAmbience
{
	Vector3 _position;
	Vector3 _normal;
	Vector2 _texCoord;
	float _ambience;

	VertexNormalTextureAmbience(Vector3 pos, Vector3 norm, Vector2 tex, float ambience)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
		_ambience = ambience;
	}

	static vector<VertexElements> Elements;
};
