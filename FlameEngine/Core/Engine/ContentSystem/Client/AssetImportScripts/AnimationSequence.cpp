#include "AnimationSequence.h"




AnimationSequence FAnimSequenceSerializer::Serialize(IOFileStream& fileStream)
{
	FAnsiString sequenceName = fileStream.ReadLengthedString<FAnsiString>();
	float sequenceLength = fileStream.Read<float>();
	uint32 numKeyFrames = fileStream.Read<uint32>();

	AnimationSequence animSeq(sequenceName, sequenceLength);

	for (int i = 0; i < numKeyFrames; i++)
	{
		AnimationKeyFrame keyFrame;

		keyFrame.timeStamp = fileStream.Read<float>();
		uint32 numTransforms = fileStream.Read<uint32>();

		keyFrame.PoseTransforms = FArray<FTransform>(numTransforms);

		for (int i = 0; i < numTransforms; i++)
		{
			FTransform ft;

			ft.Position = fileStream.Read<FVector3>();
			ft.Orientation = fileStream.Read<FQuaternion>();

			keyFrame.PoseTransforms[i] = ft;
		}

		animSeq.AddKeyFrame(keyFrame);
	}


	return animSeq;
}