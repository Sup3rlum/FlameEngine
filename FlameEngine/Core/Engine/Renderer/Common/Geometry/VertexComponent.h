#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"
#include "Core/Engine/FlameRI/FRI.h"

EXPORT(struct, FVertexElementComponentAttribute)
{
	uint32 AttribNumber;
	uint32 Type;
	bool Normalized;
	uint32 Stride;
	uint32 ByteSize;
	void* Offset;

	FVertexElementComponentAttribute(uint32 attribNumber, uint32 byteSize, uint32 type, bool norm, uint32 stride, void* offset)
	{
		AttribNumber = attribNumber;
		Type = type;
		Normalized = norm;
		Stride = stride;
		Offset = offset;
		ByteSize = byteSize;

	}

};





EXPORT(struct, FVertexComponentInterface) : public FRIArrayInterface
{



};


EXPORT(struct,  FVertexComponent_PositionNormalTexture) : public FVertexComponentInterface
{
	FVector3 Position;
	FVector3 Normal;
	FVector2 TexCoord;

	FVertexComponent_PositionNormalTexture(FVector3 pos, FVector3 norm, FVector2 tex) :
		Position(pos),
		Normal(norm),
		TexCoord(tex)
	{
	}

};

EXPORT(struct, FVertexComponent_PositionTexture) : public FVertexComponentInterface
{
	FVector3 Position;
	FVector2 TexCoord;

	FVertexComponent_PositionTexture(FVector3 pos, FVector2 tex) :
		Position(pos),
		TexCoord(tex)
	{
	}
};

EXPORT(struct, FVertexComponent_Color) : public FVertexComponentInterface
{
	FVector3 Position;
	FVector3 Color;

	FVertexComponent_Color(FVector3 pos, FVector3 col) :
		Position(pos),
		Color(col)
	{
	}
};

EXPORT(struct, FVertexComponent) : public FVertexComponentInterface
{
	FVector3 Position;

	FVertexComponent(FVector3 pos) :
		Position(pos)
	{
	}
};




EXPORT(struct, FVertexComponent_StaticFullShaded) : public FVertexComponentInterface
{
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 Bitangent;
	FVector2 TexCoord;

	FVertexComponent_StaticFullShaded(FVector3 pos, FVector3 norm, FVector3 tangent, FVector3 bitangent, FVector2 tex) :
		Position(pos),
		Normal(norm),
		Tangent(tangent),
		Bitangent(bitangent),
		TexCoord(tex)
	{
	}
};

EXPORT(struct, FVertexComponent_SkeletalFullShaded) : public FVertexComponentInterface
{
	FVector3 Position;
	FVector3 Normal;
	FVector3 Tangent;
	FVector3 Bitangent;
	FVector2 TexCoord;

	IVector4 jointIds;
	FVector4 jointWeights;

	FVertexComponent_SkeletalFullShaded(FVector3 pos, FVector3 norm, FVector3 tangent, FVector3 bitangent, FVector2 tex, IVector4 jointIds, FVector4 jointWeights) :
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

EXPORT(struct, FIndexComponent) : public FRIArrayInterface
{
	uint32 Index;

	FIndexComponent() : Index(0) {}
	FIndexComponent(uint32 index) : Index(index)
	{
	}
};