#pragma once
#include <memory>
#include <CommonUtilities/Matrix4x4.hpp>
#include <CommonUtilities/Timer.h>
#include<AssetManager/AnimationAssetHandler.h>
#include "GameObject.h"
#include<GraphicsEngine/Objects/Animation.h>
#include<GraphicsEngine/Objects/Mesh.h>

class AnimatedObject : public GameObject
{
public:
	AnimatedObject(std::shared_ptr<Animation>& anAnimation, std::shared_ptr<Mesh>& aSkeletonMesh, CommonUtilities::Vector3f aPosition);
	~AnimatedObject();
	void UpdateAnimationHierarchy(std::shared_ptr<Animation>& anAnimation, std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aParentTransform, int aCurrentID, int aCurrentFrame);
	void Update();
	const std::shared_ptr<Mesh>& GetMesh();
	const CommonUtilities::Matrix4x4<float>* GetBoneTransforms() const;
	Transform& GetTransform() override;
	void SetTransform(const Vector3f aPosition) override;
	void SetTransform(const Transform& aTransform) override;
	void SetName(const std::string aName);
	const std::string GetName() const;
private:
	std::shared_ptr<Animation> myAnimation;
	std::shared_ptr<Mesh> mySkeletalMesh;
	float myAnimationTimer;
	unsigned int myCurrentFrame;
	float myTimePerFrame;
	CommonUtilities::Matrix4x4<float> myBoneTransformCache[128];
};

