#include "AnimationComponent.h"



AnimationComponent::AnimationComponent(const AnimationComponent& other) :
	AnimationSequences(other.AnimationSequences),
	CurrentSequenceName(other.CurrentSequenceName),
	CurrentSequenceRef(other.CurrentSequenceRef),
	CurrentAnimationTime(other.CurrentAnimationTime),
	CurrentAnimationPose(other.CurrentAnimationPose)
{
}
AnimationComponent::AnimationComponent() :
	CurrentAnimationTime(0.0f),
	CurrentSequenceRef(NULL)
{

}


void AnimationComponent::AddSequence(const FString8& seqName, const AnimationSequence& seq)
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

void AnimationComponent::PlaySequence(const FString8& seqName, bool repeat, float length)
{
	if (AnimationSequences.Contains(seqName))
	{


		CurrentSequenceName = seqName;
		CurrentSequenceRef = &AnimationSequences.Get(seqName);

		CurrentAnimationTime = 0.0f;
	}
	else
	{


	}
}

void AnimationComponent::Step(float dt)
{
	if (CurrentSequenceRef)
	{
		CurrentAnimationTime += dt;
		if (CurrentAnimationTime > CurrentSequenceRef->Length)
		{
			CurrentAnimationTime = fmod(CurrentAnimationTime, CurrentSequenceRef->Length);
		}


		CurrentAnimationPose.JointTransforms = CalculatePoseJointTransforms();
	}
}

FArray<FMatrix4> AnimationComponent::CalculatePoseJointTransforms()
{
	uint32 numTransforms = CurrentSequenceRef->KeyFrames[0].PoseTransforms.Length();

	FArray<FMatrix4> PoseTransforms(numTransforms);

	for (int i = 0; i < CurrentSequenceRef->KeyFrames.Length() - 1; i++)
	{
		auto& prevKeyFrame = CurrentSequenceRef->KeyFrames[i];
		auto& nextKeyFrame = CurrentSequenceRef->KeyFrames[i + 1];

		if (prevKeyFrame.timeStamp <= CurrentAnimationTime && CurrentAnimationTime < nextKeyFrame.timeStamp)
		{
			float progression = (CurrentAnimationTime - prevKeyFrame.timeStamp) / (nextKeyFrame.timeStamp - prevKeyFrame.timeStamp);

			for (int i = 0; i < numTransforms; i++)
			{
				PoseTransforms[i] = FTransform::Interp(prevKeyFrame.PoseTransforms[i], nextKeyFrame.PoseTransforms[i], progression).GetMatrix();
			}
			break;
		}
	}


	return PoseTransforms;
}