#pragma once

//#include "../../FlameRI/FRI.h"

struct FVXGIBuffer
{
	float maxTracingDistanceGlobal;
	float bounceStrength;
	float aoFalloff;
	float aoAlpha;
	float samplingFactor;
	float coneShadowTolerance;
	float coneShadowAperture;
	float voxelVolumeDimension;
	float sceneDimension;
	float voxelScale;
	float voxelSize;

	FVector4 WorldMin;
	FVector4 WorldMax;

	float padding[1];
};
