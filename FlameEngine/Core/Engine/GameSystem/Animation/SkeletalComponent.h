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

EXPORT(struct, Skeleton)
{
	Joint* RootJoint;
	uint32 JointCount;
	FHashMap<FString8, Joint*> Joints;

	Skeleton(Joint* rootJoint, uint32 jointCount);

	Skeleton(const Skeleton& other);
	FArray<FMatrix4> GetJointTransforms();

	void ApplyPose(const SkeletonPose& pose);

private:
	void AddJointToDict(Joint* joint);
	void ApplyPoseToJoint(const SkeletonPose& pose, Joint* joint, const FMatrix4& parentTransform);
	void AddJointTransformToArray(Joint* joint, FArray<FMatrix4>& jointMatrices);

};