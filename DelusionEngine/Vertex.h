#pragma once

#include "nchfx.h"
#include "VertexElements.h"

using namespace glm;
using namespace std;

typedef struct DELUSION_DLL VertexNormalTexture
{
	vec3 _position;
	vec3 _normal;
	vec2 _texCoord;

	VertexNormalTexture(vec3 pos, vec3 norm, vec2 tex)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
	}

	static vector<VertexElements> Elements;
};

typedef struct DELUSION_DLL VertexTexture
{
	vec3 _position;
	vec2 _texCoord;

	VertexTexture(vec3 pos, vec2 tex)
	{
		_position = pos;
		_texCoord = tex;
	}

	static vector<VertexElements> Elements;
};

typedef struct DELUSION_DLL VertexColor
{
	vec3 _position;
	vec3 _color;

	VertexColor(vec3 pos, vec3 col)
	{
		_position = pos;
		_color = col;
	}

	static vector<VertexElements> Elements;
};

typedef struct DELUSION_DLL VertexNormalTextureAmbience
{
	vec3 _position;
	vec3 _normal;
	vec2 _texCoord;
	float _ambience;

	VertexNormalTextureAmbience(vec3 pos, vec3 norm, vec2 tex, float ambience)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
		_ambience = ambience;
	}

	static vector<VertexElements> Elements;
};
