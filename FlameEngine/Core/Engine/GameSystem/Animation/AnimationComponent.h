#pragma once


#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"

struct AnimationKeyFrame
{
	FArray<FTransform> PoseTransforms;
	float timeStamp;

	AnimationKeyFrame(const AnimationKeyFrame& keyFrame) :
		PoseTransforms(keyFrame.PoseTransforms),
		timeStamp(keyFrame.timeStamp)
	{
	}

	AnimationKeyFrame()
	{
	}

};


struct AnimationSequence
{
	FArray<AnimationKeyFrame> KeyFrames;
	float Length;
	FAnsiString Name;

	AnimationSequence(FAnsiString name, float length) :
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


struct AnimationComponent
{
	FHashMap<FAnsiString, AnimationSequence> AnimationSequences;
	FAnsiString CurrentSequenceName;
	AnimationSequence* CurrentSequenceRef;

	float animationTime;

	FArray<FMatrix4> CurrentBoneTransforms;


	AnimationComponent(const AnimationComponent& other) :
		AnimationSequences(other.AnimationSequences),
		CurrentSequenceName(other.CurrentSequenceName),
		CurrentSequenceRef(other.CurrentSequenceRef),
		animationTime(other.animationTime),
		CurrentBoneTransforms(other.CurrentBoneTransforms)
	{
	}
	AnimationComponent() :
		animationTime(0.0f),
		CurrentSequenceRef(NULL)
	{

	}


	void AddSequence(const FAnsiString& seqName, const AnimationSequence& seq)
	{
		if (AnimationSequences.Contains(seqName))
		{
			// TODO: Error - sequence with the same name is already defined
		}
		else
		{
			AnimationSequences.Set(seqName, seq);
		}
	}

	void PlaySequence(const FAnsiString& seqName)
	{
		if (AnimationSequences.Contains(seqName))
		{
			CurrentSequenceName = seqName;
			CurrentSequenceRef = &AnimationSequences.Get(seqName);

			animationTime = 0.0f;
		}
		else
		{
			// TODO: LOG("Unknown Animation sequence")
		}
	}

	void Step(float dt)
	{
		if (CurrentSequenceRef)
		{
			animationTime += dt;
			if (animationTime > CurrentSequenceRef->Length)
			{
				animationTime = fmod(animationTime, CurrentSequenceRef->Length);
			}


			CurrentBoneTransforms = CalculatePoseTransforms();

		}
	}

	FArray<FMatrix4> CalculatePoseTransforms()
	{
		uint32 numTransforms = CurrentSequenceRef->KeyFrames[0].PoseTransforms.Length();

		FArray<FMatrix4> PoseTransforms(numTransforms);

		for (int i = 0; i < CurrentSequenceRef->KeyFrames.Length() - 1; i++)
		{
			auto& prevKeyFrame = CurrentSequenceRef->KeyFrames[i];
			auto& nextKeyFrame = CurrentSequenceRef->KeyFrames[i + 1];

			if (prevKeyFrame.timeStamp <= animationTime && animationTime < nextKeyFrame.timeStamp)
			{
				float progression = (animationTime - prevKeyFrame.timeStamp) / (nextKeyFrame.timeStamp - prevKeyFrame.timeStamp);

				for (int i = 0; i < numTransforms; i++)
				{
					PoseTransforms[i] = FTransform::Interp(prevKeyFrame.PoseTransforms[i], nextKeyFrame.PoseTransforms[i], progression).GetMatrix();
				}
				break;
			}
		}


		return PoseTransforms;
	}

};