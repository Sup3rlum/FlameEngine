#pragma once

#include "Core/Common/CoreCommon.h"
#include "Core/Math/Module.h"

#include "AnimationComponent.h"

struct Joint
{
	FArray<Joint*> childrenJoints;
	uint32 ID;
	FAnsiString Name;

	FMatrix4 animatedTransform;

	FMatrix4 localBindTransform;
	FMatrix4 inverseBindTransform;


	void CalculateInverseBind(FMatrix4 parentBindTransform)
	{
		FMatrix4 bindTransform = parentBindTransform * localBindTransform;
		inverseBindTransform = FMatrix4::Inverse(bindTransform);

		for (auto child : childrenJoints)
		{
			child->CalculateInverseBind(bindTransform);
		}

	}

	Joint(uint32 index, FAnsiString name, FMatrix4 localTransform) :
		ID(index),
		Name(name),
		localBindTransform(localTransform)
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


struct SkeletalComponent
{
	Joint* RootJoint;
	uint32 JointCount;


	SkeletalComponent(Joint* rootJoint, uint32 jointCount) :
		RootJoint(rootJoint),
		JointCount(jointCount)
	{}


	SkeletalComponent(const SkeletalComponent& other) :
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



	void ApplyPose(AnimationComponent& animation)
	{
		if (animation.CurrentSequenceRef)
		{
			ApplyPoseToJoint(animation, RootJoint, FMatrix4(1));
		}
	}


	void ApplyPoseToJoint(AnimationComponent& animation, Joint* joint, FMatrix4 parentTransform)
	{
		FMatrix4 currentLocalTransform = animation.CurrentBoneTransforms[joint->ID];
		FMatrix4 currentTransform = parentTransform * currentLocalTransform;

		joint->animatedTransform = currentTransform * joint->inverseBindTransform;

		for (auto j : joint->childrenJoints)
		{
			ApplyPoseToJoint(animation, j, currentTransform);
		}

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