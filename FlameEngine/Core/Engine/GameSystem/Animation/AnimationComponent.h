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
	FString8 Name;

	AnimationSequence(const FString8& name, float length) :
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
	FHashMap<FString8, AnimationSequence> AnimationSequences;
	FString8 CurrentSequenceName;
	AnimationSequence* CurrentSequenceRef;

	float animationTime;
	SkeletonPose CurrentAnimationPose;


	AnimationComponent(const AnimationComponent & other);
	AnimationComponent();

	void AddSequence(const FString8& seqName, const AnimationSequence& seq);

	void PlaySequence(const FString8& seqName);
	void Step(float dt);

	FArray<FMatrix4> CalculatePoseJointTransforms();
};