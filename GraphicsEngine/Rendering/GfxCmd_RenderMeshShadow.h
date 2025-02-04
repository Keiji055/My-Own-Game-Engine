#pragma once
#include "../GfxCmd_RenderMesh.h"
#include "../GraphicsEngine.h"

class GfxCmd_RenderMeshShadow : public GfxCmd_RenderMesh
{
public:
	GfxCmd_RenderMeshShadow(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform);
	GfxCmd_RenderMeshShadow(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform, const CommonUtilities::Matrix4x4<float>* aBoneTransformList, unsigned aNumBones);
	void SetMaterialResource(int aMaterialIndex) override;
};
