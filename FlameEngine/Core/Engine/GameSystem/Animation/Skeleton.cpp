#include "SkeletalComponent.h"

Skeleton::Skeleton(Joint* rootJoint, uint32 jointCount) :
	RootJoint(rootJoint),
	JointCount(jointCount)
{
	AddJointToDict(rootJoint);
}


Skeleton::Skeleton(const Skeleton& other) :
	RootJoint(other.RootJoint),
	JointCount(other.JointCount)
{
	AddJointToDict(other.RootJoint);
}

FArray<FMatrix4> Skeleton::GetJointTransforms()
{
	FArray<FMatrix4> jointMatrices;
	jointMatrices.Resize(JointCount);
	AddJointTransformToArray(RootJoint, jointMatrices);
	return jointMatrices;
}



void Skeleton::ApplyPose(const SkeletonPose& pose)
{
	if (pose.JointTransforms.Length() > 0)
	{
		ApplyPoseToJoint(pose, RootJoint, FMatrix4(1));
	}
}


void Skeleton::ApplyPoseToJoint(const SkeletonPose& pose, Joint* joint, const FMatrix4& parentTransform)
{
	FMatrix4 currentTransform = parentTransform * pose.JointTransforms[joint->ID];

	for (auto j : joint->childrenJoints)
	{
		ApplyPoseToJoint(pose, j, currentTransform);
	}

	joint->animatedTransform = currentTransform * joint->inverseBindTransform;
}


void Skeleton::AddJointTransformToArray(Joint* joint, FArray<FMatrix4>& jointMatrices)
{
	jointMatrices[joint->ID] = joint->animatedTransform;

	for (auto joint : joint->childrenJoints)
	{
		AddJointTransformToArray(joint, jointMatrices);
	}
}

void Skeleton::AddJointToDict(Joint* joint)
{
	Joints[joint->Name] = joint;

	for (auto child : joint->childrenJoints)
	{
		AddJointToDict(child);
	}
}