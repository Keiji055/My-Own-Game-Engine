#include "GraphicsEngine.pch.h"
#include "GfxCmd_RenderMeshShadow.h"


GfxCmd_RenderMeshShadow::GfxCmd_RenderMeshShadow(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform)
	: GfxCmd_RenderMesh(aMesh, aTransform)
{
}

GfxCmd_RenderMeshShadow::GfxCmd_RenderMeshShadow(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform, const CommonUtilities::Matrix4x4<float>* aBoneTransformList, unsigned aNumBones)
	: GfxCmd_RenderMesh(aMesh, aTransform, aBoneTransformList, aNumBones)
{
}

void GfxCmd_RenderMeshShadow::SetMaterialResource(int aMaterialIndex)
{
	GfxCmd_RenderMesh::SetMaterialResource(aMaterialIndex);
	RHI::Context->PSSetShader(nullptr, nullptr, 0);
}
