#include "AnimationSequence.h"


void PrintDebugQuat(const FQuaternion& q)
{
	printf("%f %f %f %f\n", q.r, q.i, q.j, q.k);
}

AnimationSequence TContentSerializer<AnimationSequence>::Serialize(Stream& fileStream)
{
	FString8 sequenceName = fileStream.ReadLengthedString<FString8>();
	float sequenceLength = fileStream.Read<float>();
	uint32 numKeyFrames = fileStream.Read<uint32>();

	AnimationSequence animSeq(sequenceName, sequenceLength);

	for (int i = 0; i < numKeyFrames; i++)
	{
		AnimationKeyFrame keyFrame;

		keyFrame.timeStamp = fileStream.Read<float>();
		uint32 numTransforms = fileStream.Read<uint32>();

		keyFrame.PoseTransforms = FArray<FTransform>(numTransforms);

		printf("Frame: %f\n", keyFrame.timeStamp);

		for (int i = 0; i < numTransforms; i++)
		{
			FMatrix4 tr = fileStream.Read<FMatrix4>();

			printf("Joint: %d\n", i);
			wprintf(L"%s", tr.ToString().ToPlatformString());

			keyFrame.PoseTransforms[i] = FTransform(tr);
		}

		animSeq.AddKeyFrame(keyFrame);
	}


	return animSeq;
}