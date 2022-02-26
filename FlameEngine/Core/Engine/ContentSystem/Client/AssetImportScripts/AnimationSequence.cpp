#include "AnimationSequence.h"


void PrintDebugMatrix2(const FMatrix4& a)
{

	for (int j = 0; j < 4; j++)
	{
		printf("\t{ %f %f %f %f }\n", a[j].x, a[j].y, a[j].z, a[j].w);
	}
}

AnimationSequence TContentSerializer<AnimationSequence>::Serialize(IOFileStream& fileStream)
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

		//printf("Frame: %f\n", keyFrame.timeStamp);

		for (int i = 0; i < numTransforms; i++)
		{
			FMatrix4 tr = fileStream.Read<FMatrix4>();

			//printf("Joint: %d\n", i);
			//PrintDebugMatrix2(tr);

			keyFrame.PoseTransforms[i] = FTransform(tr);
		}

		animSeq.AddKeyFrame(keyFrame);
	}


	return animSeq;
}