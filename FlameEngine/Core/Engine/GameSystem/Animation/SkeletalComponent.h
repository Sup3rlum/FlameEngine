#pragma once


#include "SkeletonPose.h"


struct Joint
{
	FArray<Joint*> childrenJoints;
	uint32 ID;
	FString8 Name;

	FMatrix4 animatedTransform;
	FMatrix4 inverseBindTransform;

	Joint(uint32 index, FString8 name, FMatrix4 inverseBindTransform) :
		ID(index),
		Name(name),
		inverseBindTransform(inverseBindTransform)
	{

	}


	Joint(const Joint& other) :
		childrenJoints(other.childrenJoints),
		ID(other.ID),
		Name(other.Name),
		animatedTransform(other.animatedTransform),
		inverseBindTransform(other.inverseBindTransform)
	{

	}

	void AddChildJoint(Joint* child)
	{
		childrenJoints.Add(child);
	}

};

struct Skeleton
{
	Joint* RootJoint;
	uint32 JointCount;


	Skeleton(Joint* rootJoint, uint32 jointCount) :
		RootJoint(rootJoint),
		JointCount(jointCount)
	{}


	Skeleton(const Skeleton& other) :
		RootJoint(other.RootJoint),
		JointCount(other.JointCount)
	{

	}

	FArray<FMatrix4> GetJointTransforms()
	{
		FArray<FMatrix4> jointMatrices(JointCount);
		AddJointTransformToArray(RootJoint, jointMatrices);
		return jointMatrices;
	}



	void ApplyPose(const SkeletonPose& pose)
	{
		ApplyPoseToJoint(pose, RootJoint, FMatrix4(1));
	}


	void ApplyPoseToJoint(const SkeletonPose& pose, Joint* joint, const FMatrix4& parentTransform)
	{
		FMatrix4 currentTransform = pose.JointTransforms[joint->ID] * parentTransform;

		for (auto j : joint->childrenJoints)
		{
			ApplyPoseToJoint(pose, j, currentTransform);
		}

		joint->animatedTransform = currentTransform * joint->inverseBindTransform;
	}



private:

	void AddJointTransformToArray(Joint* joint, FArray<FMatrix4>& jointMatrices)
	{
		jointMatrices[joint->ID] = joint->animatedTransform;

		for (auto joint : joint->childrenJoints)
		{
			AddJointTransformToArray(joint, jointMatrices);
		}
	}

};