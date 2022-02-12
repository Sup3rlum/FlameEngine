#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"
#include "SkeletonPose.h"

#include <iostream>

struct AnimationKeyFrame
{
	FArray<FTransform> PoseTransforms;
	float timeStamp;

	AnimationKeyFrame(const AnimationKeyFrame& keyFrame) :
		PoseTransforms(keyFrame.PoseTransforms),
		timeStamp(keyFrame.timeStamp)
	{
	}

	AnimationKeyFrame() :
		timeStamp(0)
	{
	}

};


struct AnimationSequence
{
	FArray<AnimationKeyFrame> KeyFrames;
	float Length;
	FAnsiString Name;

	AnimationSequence(const FAnsiString& name, float length) :
		Name(name),
		Length(length)
	{
	}

	AnimationSequence(const AnimationSequence& other) :
		KeyFrames(other.KeyFrames),
		Length(other.Length),
		Name(other.Name)
	{

	}

	void AddKeyFrame(const AnimationKeyFrame& keyFrame)
	{
		KeyFrames.Add(keyFrame);
	}

};


EXPORT(struct, AnimationComponent)
{
	FHashMap<FAnsiString, AnimationSequence> AnimationSequences;
	FAnsiString CurrentSequenceName;
	AnimationSequence* CurrentSequenceRef;

	float animationTime;
	SkeletonPose CurrentAnimationPose;


	AnimationComponent(const AnimationComponent & other);
	AnimationComponent();

	void AddSequence(const FAnsiString& seqName, const AnimationSequence& seq);

	void PlaySequence(const FAnsiString& seqName);
	void Step(float dt);

	FArray<FMatrix4> CalculatePoseJointTransforms();
};