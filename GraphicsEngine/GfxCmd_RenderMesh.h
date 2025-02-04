#pragma once
#include "GraphicsCommand.h"
#include <memory>
#include<GraphicsEngine/Objects/Mesh.h>
#include<GraphicsEngine/Rendering/ObjectBuffer.h>
#include<GraphicsEngine/Rendering/Vertex.h>
#include <CommonUtilities/Matrix4x4.hpp>
class GfxCmd_RenderMesh : public GraphicsCommand
{
	CommonUtilities::Matrix4x4<float> myTransformMatrix;
	CommonUtilities::Matrix4x4<float> myBoneTransforms[128]{};
	bool myHasBones = false;
	std::vector<Mesh::Element> myElementData;/*
	std::vector<CommonUtilities::Transform> myMeshInstances;*/
	ComPtr<ID3D11Buffer> myInstances;
	unsigned myInstanceCount;

public:
	GfxCmd_RenderMesh() = default;
	GfxCmd_RenderMesh(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform);
	GfxCmd_RenderMesh(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform, const CommonUtilities::Matrix4x4<float>* aBoneTransformList, unsigned aNumBones);
	virtual void SetMaterialResource(int aMaterialIndex);
	void Execute() override;
};
