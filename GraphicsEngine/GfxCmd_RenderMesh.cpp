#include "GraphicsEngine.pch.h"
#include "GfxCmd_RenderMesh.h"
#include <InterOp/RHI.h>
#include <GraphicsEngine/Rendering/ConstantBuffer.h>

GfxCmd_RenderMesh::GfxCmd_RenderMesh(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform)
{
	myElementData = aMesh->GetElement();
	if (aMesh->HasInstances())
	{
		myInstances = aMesh->GetInstanceBuffer();
		myInstanceCount = aMesh->GetInstanceCount();
	}
	myTransformMatrix = aTransform;
	myHasBones = false;
}

GfxCmd_RenderMesh::GfxCmd_RenderMesh(const std::shared_ptr<Mesh>& aMesh, const CommonUtilities::Matrix4x4<float>& aTransform, const CommonUtilities::Matrix4x4<float>* aBoneTransformList, unsigned aNumBones)
{
	myElementData = aMesh->GetElement();
	myTransformMatrix = aTransform;
	myHasBones = true;
	if (aMesh->HasInstances())
	{
		myInstances = aMesh->GetInstanceBuffer();
		myInstanceCount = aMesh->GetInstanceCount();
	}
	for (unsigned int index = 0; index < aNumBones; index++)
	{
		myBoneTransforms[index] = aBoneTransformList[index];
	}
}

void GfxCmd_RenderMesh::SetMaterialResource(int aMaterialIndex)
{
	if (const std::shared_ptr<Material> material = myElementData[aMaterialIndex].myMaterial.lock())
	{
		material->SetAsResource();
	}
}

void GfxCmd_RenderMesh::Execute()
{
	for (int i = 0; i < myElementData.size(); i++)
	{
		ObjectBuffer& objectBuffer = GetObjectBuffer();
		objectBuffer.Data.Transform = myTransformMatrix;
		SetMaterialResource(i);
		if (myInstances != nullptr)
		{
			objectBuffer.Data.IsInstanced = true;
		}
		else
		{
			objectBuffer.Data.IsInstanced = false;
		}
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
		
		RHI::UpdateConstantBufferData(objectBuffer);
		RHI::SetConstantBuffer(PIPELINE_STAGE::PIPELINE_STAGE_VERTEX_SHADER, 1, objectBuffer);

		if (myInstances != nullptr)
		{
			const std::vector<ComPtr<ID3D11Buffer>> vxBuffers
			{
				myElementData[i].myVertexBuffer,
				myInstances
			};

			const std::vector<unsigned> vxBufferStrides
			{
				myElementData[i].myStride,
				sizeof(CommonUtilities::Matrix4f)
			};

			RHI::ConfigureInputAssembler(myElementData[i].myPrimitiveTopology, vxBuffers, myElementData[i].myIndexBuffer,
				vxBufferStrides, Vertex::InputLayout);
			RHI::DrawIndexedInstanced(myElementData[i].myNumIndices, myInstanceCount);
		}
		else
		{
			RHI::ConfigureInputAssembler(myElementData[i].myPrimitiveTopology, myElementData[i].myVertexBuffer, 
				myElementData[i].myIndexBuffer, myElementData[i].myStride, Vertex::InputLayout);
			RHI::DrawIndexed(myElementData[i].myNumIndices);
		}
	}
}