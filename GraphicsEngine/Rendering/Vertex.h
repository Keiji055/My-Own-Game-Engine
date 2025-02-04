#pragma once
#include <wrl.h>
#include <CommonUtilities/Vector.hpp>
#include <vector>
#include <random>
#include <TgaFbxStructs.h>
#include <d3d11.h>
using namespace Microsoft::WRL;

// We'll be writing this a lot so easier
// to just typedef it here.
typedef std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementList;

struct Vertex
{
	CommonUtilities::Vector4f myPosition;
	CommonUtilities::Vector4f myColor;
	CommonUtilities::Vector2f myUV0;
	CommonUtilities::Vector2f myUV1;
	CommonUtilities::Vector4<unsigned int> myBoneIDs;
	CommonUtilities::Vector4f myBoneWeights;
	CommonUtilities::Vector3f myNormal;
	CommonUtilities::Vector3f myTangent;


	Vertex(float aX, float aY, float aZ, float aUVx, float aUVy, float aNormalX, float aNormalY, float aNormalZ, float aTangentX, float aTangentY, float aTangentZ, 
		   float aR, float aG, float aB, float aA = 1)
	{
		myPosition.x = aX;
		myPosition.y = aY;
		myPosition.z = aZ;
		myPosition.w = 1.0f;

		myUV0.x = aUVx;
		myUV0.y = aUVy;

		myUV1.x = aUVx;
		myUV1.y = aUVy;

		myColor.x = aR;
		myColor.y = aG;
		myColor.z = aB;
		myColor.w = aA;

		myNormal.x = aNormalX;
		myNormal.y = aNormalY;
		myNormal.z = aNormalZ;

		myTangent.x = aTangentX;
		myTangent.y = aTangentY;
		myTangent.z = aTangentZ;
	}

	static const InputElementList InputLayoutDefinition;
	static ComPtr<ID3D11InputLayout> InputLayout; 

	Vertex(TGA::FBX::Vertex aTGAVertex)
	{
		myPosition.x = aTGAVertex.Position[0];
		myPosition.y = aTGAVertex.Position[1];
		myPosition.z = aTGAVertex.Position[2];
		myPosition.w = 1.0f;

		myColor.x = 1.0f;
		myColor.y = 1.0f;
		myColor.z = 1.0f;
		myColor.w = 1.0f;

		myUV0.x = aTGAVertex.UVs[0][0];
		myUV0.y = aTGAVertex.UVs[0][1];

		myUV1.x = aTGAVertex.UVs[0][0];
		myUV1.y = aTGAVertex.UVs[0][1];

		myBoneIDs.x = aTGAVertex.BoneIDs[0];
		myBoneIDs.y = aTGAVertex.BoneIDs[1];
		myBoneIDs.z = aTGAVertex.BoneIDs[2];
		myBoneIDs.w = aTGAVertex.BoneIDs[3];

		myBoneWeights.x = aTGAVertex.BoneWeights[0];
		myBoneWeights.y = aTGAVertex.BoneWeights[1];
		myBoneWeights.z = aTGAVertex.BoneWeights[2];
		myBoneWeights.w = aTGAVertex.BoneWeights[3];

		myNormal.x = aTGAVertex.Normal[0];
		myNormal.y = aTGAVertex.Normal[1];
		myNormal.z = aTGAVertex.Normal[2];

		myTangent.x = aTGAVertex.Tangent[0];
		myTangent.y = aTGAVertex.Tangent[1];
		myTangent.z = aTGAVertex.Tangent[2];
	}
};

