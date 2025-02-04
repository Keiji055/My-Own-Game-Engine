#pragma once
#include "../GraphicsEngine.h"
#include <GraphicsCommand.h>


class GfxCmd_RenderDeferredDataPass : public GraphicsCommand
{
	CommonUtilities::Matrix4x4<float> myTransformMatrix;
	CommonUtilities::Matrix4x4<float> myBoneTransforms[128]{};
	bool myHasBones = false;
	std::vector<Mesh::Element> myElementData;
public:
	GfxCmd_RenderDeferredDataPass(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform);
	GfxCmd_RenderDeferredDataPass(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform, const CommonUtilities::Matrix4x4<float>* aBoneTransformList, unsigned aNumBones);
	void SetMaterialResource(int aMaterialIndex);
	void Execute();
};