#include "AnimationComponent.h"



AnimationComponent::AnimationComponent(const AnimationComponent& other) :
	AnimationSequences(other.AnimationSequences),
	CurrentSequenceName(other.CurrentSequenceName),
	CurrentSequenceRef(other.CurrentSequenceRef),
	animationTime(other.animationTime),
	CurrentAnimationPose(other.CurrentAnimationPose)
{
}
AnimationComponent::AnimationComponent() :
	animationTime(0.0f),
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

void AnimationComponent::PlaySequence(const FString8& seqName)
{

	auto b = AnimationSequences.Begin();
	auto e = AnimationSequences.End();

	if (AnimationSequences.Contains(seqName))
	{
		CurrentSequenceName = seqName;
		CurrentSequenceRef = &AnimationSequences.Get(seqName);

		animationTime = 0.0f;
	}
	else
	{
		//std::wcout << L"Unrecognized sequence: " << seqName.ToPlatformString() << std::endl;

	}
}

void AnimationComponent::Step(float dt)
{
	if (CurrentSequenceRef)
	{
		animationTime += dt;
		if (animationTime > CurrentSequenceRef->Length)
		{
			animationTime = fmod(animationTime, CurrentSequenceRef->Length);
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