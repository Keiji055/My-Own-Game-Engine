#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(std::shared_ptr<Animation>& anAnimation, std::shared_ptr<Mesh>& aSkeletonMesh, CommonUtilities::Vector3f aPosition)
{
	myAnimation = anAnimation;
	mySkeletalMesh = aSkeletonMesh;
	myCurrentFrame = 0;
	myTimePerFrame = 1.0f / anAnimation->GetFPS();
	myAnimationTimer = 0;
	myData.Transform.SetPosition(aPosition);
}

AnimatedObject::~AnimatedObject()
{
}

void AnimatedObject::Update()
{
	if (myCurrentFrame >= myAnimation->GetLength() - 1)
	{
		myCurrentFrame = 0;
	}
	myAnimationTimer += CommonUtilities::Timer::GetDeltaTime();
	if (myAnimationTimer >= myTimePerFrame)
	{
		myAnimationTimer = 0;
		myCurrentFrame++;
		UpdateAnimationHierarchy(myAnimation, mySkeletalMesh, myAnimation->GetFrames()[0].LocalTransforms.at(mySkeletalMesh->GetSkeleton().bones[0].name), 0, myCurrentFrame);
	}
}


const std::shared_ptr<Mesh>& AnimatedObject::GetMesh()
{
	return mySkeletalMesh;
}

const CommonUtilities::Matrix4x4<float>* AnimatedObject::GetBoneTransforms() const
{
	return myBoneTransformCache;
}

Transform& AnimatedObject::GetTransform()
{
	return myData.Transform;
}

void AnimatedObject::SetTransform(const Vector3f aPosition)
{
	myData.Transform.SetPosition(aPosition);
}

void AnimatedObject::SetTransform(const Transform& aTransform)
{
	myData.Transform = aTransform;
}

void AnimatedObject::SetName(const std::string aName)
{
	myData.Name = aName;
}

const std::string AnimatedObject::GetName() const
{
	return myData.Name;
}

void AnimatedObject::UpdateAnimationHierarchy(std::shared_ptr<Animation>& anAnimation, std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aParentTransform, int aCurrentID, int aCurrentFrame)
{
	CommonUtilities::Matrix4x4<float> frameTransform = anAnimation->GetFrames()[aCurrentFrame].LocalTransforms.at(aMesh->GetSkeleton().bones[aCurrentID].name);

	CommonUtilities::Matrix4x4<float> boneTransform = aParentTransform * frameTransform;

	myBoneTransformCache[aCurrentID] = boneTransform * aMesh->GetSkeleton().bones[aCurrentID].bindPoseInverse;

	for (size_t i = 0; i < aMesh->GetSkeleton().bones[aCurrentID].children.size(); i++)
	{
		UpdateAnimationHierarchy(anAnimation, aMesh, boneTransform, aMesh->GetSkeleton().bones[aCurrentID].children[i], aCurrentFrame);
	}
}
