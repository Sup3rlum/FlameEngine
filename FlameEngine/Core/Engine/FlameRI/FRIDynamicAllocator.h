#pragma once


#include "Core/Common/CoreCommon.h"
#include "FRIResource.h"

class FRIDynamicAllocator
{
	virtual FResourceVertexBuffer RHICreateVertexBuffer(uint32 Size, uint32 Usage, FResourceCreationDescriptor resourceDescriptor);


	virtual FResourceVertexShader	FRICreateVertexShader(FArray<byte> binCode);
	virtual FResourcePixelShader	FRICreatePixelShader(FArray<byte> binCode);
	virtual FResourceGeometryShader FRICreateGeometryShader(FArray<byte> binCode);
	virtual FResourceHullShader		FRICreateHullShader(FArray<byte> binCode);
	virtual FResourceDomainShader	FRICreateDomainShader(FArray<byte> binCode);
	virtual FResourceComputeShader	FRICreateComputeShader(FArray<byte> binCode);

	virtual FResourceTexture2D		FRICreateTexture2D(uint32 width, uint32 height);
	virtual FResourceTexture3D		FRICreateTexture3D(uint32 width, uint32 height, uint32 depth);

	virtual FResourceFrameBuffer	FRICreateFrameBuffer();


};