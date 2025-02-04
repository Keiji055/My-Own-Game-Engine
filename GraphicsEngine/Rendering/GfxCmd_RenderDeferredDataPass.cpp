#include "GraphicsEngine.pch.h"
#include "GfxCmd_RenderDeferredDataPass.h"
#include <Shaders/Include/GBufferPS.h>

GfxCmd_RenderDeferredDataPass::GfxCmd_RenderDeferredDataPass(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform)
{
	myElementData = aMesh->GetElement();

	myTransformMatrix = aTransform;
	myHasBones = false;
}

GfxCmd_RenderDeferredDataPass::GfxCmd_RenderDeferredDataPass(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform, 
	const CommonUtilities::Matrix4x4<float>* aBoneTransformList, unsigned aNumBones)
{
	myElementData = aMesh->GetElement();
	myTransformMatrix = aTransform;
	myHasBones = true;
	for (unsigned int index = 0; index < aNumBones; index++)
	{
		myBoneTransforms[index] = aBoneTransformList[index];
	}
}

void GfxCmd_RenderDeferredDataPass::SetMaterialResource(int aMaterialIndex)
{
	if (const std::shared_ptr<Material> material = myElementData[aMaterialIndex].myMaterial.lock())
	{
		material->SetAsResource();
	}
}

void GfxCmd_RenderDeferredDataPass::Execute()
{
	/*ComPtr<ID3D11PixelShader> deferredPS;
	RHI::CreatePixelShader(deferredPS,
		BuiltIn_GBufferPS_ByteCode,
		sizeof(BuiltIn_GBufferPS_ByteCode));*/

	for (int i = 0; i < myElementData.size(); i++)
	{
		ObjectBuffer& objectBuffer = GetObjectBuffer();
		objectBuffer.Data.Transform = myTransformMatrix;
		SetMaterialResource(i);
		objectBuffer.Data.IsInstanced = false;
		if (myHasBones)
		{
			objectBuffer.Data.HasBones = true;
			for (size_t i = 0; i < sizeof(myBoneTransforms) / sizeof(myBoneTransforms[0]); i++)
			{
				objectBuffer.Data.BoneTransform[i] = myBoneTransforms[i];
			}
		}
		else
		{
			objectBuffer.Data.HasBones = false;
		}

		RHI::SetPixelShader(GraphicsEngine::Get().GetDeferredPixelShader());

		RHI::UpdateConstantBufferData(objectBuffer);
		RHI::SetConstantBuffer(PIPELINE_STAGE::PIPELINE_STAGE_VERTEX_SHADER, 1, objectBuffer);
		RHI::ConfigureInputAssembler(myElementData[i].myPrimitiveTopology, myElementData[i].myVertexBuffer, 
			myElementData[i].myIndexBuffer, myElementData[i].myStride, Vertex::InputLayout);
		RHI::DrawIndexed(myElementData[i].myNumIndices);
	}
}
