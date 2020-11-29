#pragma once

#include "../../dll/nchfx.h"
#include "VertexElements.h"
#include "../../Mathematics/Module.h"

using namespace std;


EXPORT(struct,  VertexNormalTexture)
{
	fVector3 _position;
	fVector3 _normal;
	fVector2 _texCoord;

	VertexNormalTexture(fVector3 pos, fVector3 norm, fVector2 tex)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
	}

	static vector<VertexElements> Elements;
};

EXPORT(struct,  VertexTexture)
{
	fVector3 _position;
	fVector2 _texCoord;

	VertexTexture(fVector3 pos, fVector2 tex)
	{
		_position = pos;
		_texCoord = tex;
	}

	static vector<VertexElements> Elements;
};

EXPORT(struct,  VertexColor)
{
	fVector3 _position;
	fVector3 _color;

	VertexColor(fVector3 pos, fVector3 col)
	{
		_position = pos;
		_color = col;
	}

	static vector<VertexElements> Elements;
};

EXPORT(struct,  VertexNormalTextureAmbience)
{
	fVector3 _position;
	fVector3 _normal;
	fVector2 _texCoord;
	float _ambience;

	VertexNormalTextureAmbience(fVector3 pos, fVector3 norm, fVector2 tex, float ambience)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
		_ambience = ambience;
	}

	static vector<VertexElements> Elements;
};
/*
EXPORT(struct,  AnimatedModelVertex)
{

	// Skin data
	fVector3 _position;
	fVector3 _normal;
	fVector2 _texCoord;

	// Skeletal animation Data
	iVector4 _jointIds;
	fVector4 _jointWeights;

	AnimatedModelVertex(fVector3 pos, fVector3 norm, fVector2 tex, iVector4 jointIds, fVector4 jointWeights)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
		_jointIds = jointIds;
		_jointWeights = jointWeights;
	}

	static vector<VertexElements> Elements;
};*/