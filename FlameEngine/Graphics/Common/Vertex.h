#pragma once

#include "../../dll/nchfx.h"
#include "VertexElements.h"
#include "../../Mathematics/Module.h"

using namespace std;


EXPORT(struct,  VertexNormalTexture)
{
	FVector3 _position;
	FVector3 _normal;
	FVector2 _texCoord;

	VertexNormalTexture(FVector3 pos, FVector3 norm, FVector2 tex)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
	}

	static vector<VertexElements> Elements;
};

EXPORT(struct,  VertexTexture)
{
	FVector3 _position;
	FVector2 _texCoord;

	VertexTexture(FVector3 pos, FVector2 tex)
	{
		_position = pos;
		_texCoord = tex;
	}

	static vector<VertexElements> Elements;
};

EXPORT(struct,  VertexColor)
{
	FVector3 _position;
	FVector3 _color;

	VertexColor(FVector3 pos, FVector3 col)
	{
		_position = pos;
		_color = col;
	}

	static vector<VertexElements> Elements;
};

EXPORT(struct,  VertexNormalTextureAmbience)
{
	FVector3 _position;
	FVector3 _normal;
	FVector2 _texCoord;
	float _ambience;

	VertexNormalTextureAmbience(FVector3 pos, FVector3 norm, FVector2 tex, float ambience)
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
	FVector3 _position;
	FVector3 _normal;
	FVector2 _texCoord;

	// Skeletal animation Data
	iVector4 _jointIds;
	FVector4 _jointWeights;

	AnimatedModelVertex(FVector3 pos, FVector3 norm, FVector2 tex, iVector4 jointIds, FVector4 jointWeights)
	{
		_position = pos;
		_normal = norm;
		_texCoord = tex;
		_jointIds = jointIds;
		_jointWeights = jointWeights;
	}

	static vector<VertexElements> Elements;
};*/