#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"
#include "Core/Engine/FlameRI/FRI.h"

class InputLayouts
{
public:
	inline static FArray<FRIInputAttribute> StaticLit =
	{
		FRIInputAttribute("POSITION",	3, EFRIAttributeType::Float, EFRIBool::False, 56, 0),
		FRIInputAttribute("NORMAL",		3, EFRIAttributeType::Float, EFRIBool::False, 56, 12),
		FRIInputAttribute("TANGENT",	3, EFRIAttributeType::Float, EFRIBool::False, 56, 24),
		FRIInputAttribute("BITANGENT",	3, EFRIAttributeType::Float, EFRIBool::False, 56, 36),
		FRIInputAttribute("TEXCOORD",	2, EFRIAttributeType::Float, EFRIBool::False, 56, 48)
	};

	inline static FArray<FRIInputAttribute> SkinnedLit =
	{
		FRIInputAttribute("POSITION",		3, EFRIAttributeType::Float, EFRIBool::False, 88, 0),
		FRIInputAttribute("NORMAL",			3, EFRIAttributeType::Float, EFRIBool::False, 88, 12),
		FRIInputAttribute("TANGENT",		3, EFRIAttributeType::Float, EFRIBool::False, 88, 24),
		FRIInputAttribute("BITANGENT",		3, EFRIAttributeType::Float, EFRIBool::False, 88, 36),
		FRIInputAttribute("TEXCOORD",		2, EFRIAttributeType::Float, EFRIBool::False, 88, 48),
		FRIInputAttribute("JOINT_INDICES",	4, EFRIAttributeType::Int,	 EFRIBool::False, 88, 56),
		FRIInputAttribute("JOINT_WEIGHTS",	4, EFRIAttributeType::Float, EFRIBool::False, 88, 72)

	};

	inline static FArray<FRIInputAttribute> PositionTexture =
	{
		FRIInputAttribute("POSITION", 3, EFRIAttributeType::Float, EFRIBool::False, 56, 0),
		FRIInputAttribute("TEXCOORD", 2, EFRIAttributeType::Float, EFRIBool::False, 56, 48)
	};
};


struct FVertex_PositionNormalTexture
{
	FVector3 Position;
	FVector3 Normal;
	FVector2 TexCoord;

	FVertex_PositionNormalTexture(FVector3 pos, FVector3 norm, FVector2 tex) :
		Position(pos),
		Normal(norm),
		TexCoord(tex)
	{
	}

};

struct FVertex_PositionTexture
{
	FVector3 Position;
	FVector2 TexCoord;

	FVertex_PositionTexture(FVector3 pos, FVector2 tex) :
		Position(pos),
		TexCoord(tex)
	{
	}
};

struct FVertex_PositionColor
{
	FVector3 Position;
	FVector3 Color;

	FVertex_PositionColor(FVector3 pos, FVector3 col) :
		Position(pos),
		Color(col)
	{
	}
};

struct FVertex_Position
{
	FVector3 Position;

	FVertex_Position(FVector3 pos) :
		Position(pos)
	{
	}
};


struct FVertex_StaticLit
{
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 Bitangent;
	FVector2 TexCoord;

	FVertex_StaticLit(FVector3 pos, FVector3 norm, FVector3 tangent, FVector3 bitangent, FVector2 tex) :
		Position(pos),
		Normal(norm),
		Tangent(tangent),
		Bitangent(bitangent),
		TexCoord(tex)
	{
	}
};

struct FVertex_SkinnedLit
{
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 Bitangent;
	FVector2 TexCoord;

	IVector4 jointIds;
	FVector4 jointWeights;

	FVertex_SkinnedLit(FVector3 pos, FVector3 norm, FVector3 tangent, FVector3 bitangent, FVector2 tex, IVector4 jointIds, FVector4 jointWeights) :
		Position(pos),
		Normal(norm),
		Tangent(tangent),
		Bitangent(bitangent),
		TexCoord(tex),

		jointIds(jointIds),
		jointWeights(jointWeights)
	{
	}
};

struct FIndexComponent
{
	uint32 Index;

	FIndexComponent() : Index(0) {}
	FIndexComponent(uint32 index) : Index(index)
	{
	}
};