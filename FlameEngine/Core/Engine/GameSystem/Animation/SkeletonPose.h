#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"


struct SkeletonPose
{
	// Object/Skeleton Space
	FArray<FMatrix4> JointTransforms;


	SkeletonPose()
	{}

	SkeletonPose(const SkeletonPose& otherPose) :
		JointTransforms(otherPose.JointTransforms)
	{}
};