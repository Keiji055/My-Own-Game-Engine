#include <GraphicsEngine.pch.h>
#include "Mesh.h"
#include <unordered_map>
#include <InterOp/RHI.h>
#include <CommonUtilities/Vector.hpp>
#include <cmath>
#include <TGAFbx.h>

#define Pi 3.14159265359

void Mesh::CreateCube(Element& anElement)
{
	anElement.myMdlVertices = {

		// Back
		{50.0f,50.0f,50.0f,		1,0,	0,0,1, -1,0,0,	 1, 1, 1, 1},// 0
		{-50.0f,50.0f,50.0f,	0,0,	0,0,1, -1,0,0,   1, 1, 1, 1},// 1
		{50.0f,-50.0f,50.0f,	1,1,	0,0,1, -1,0,0,   1, 1, 1, 1},// 2
		{-50.0f,-50.0f,50.0f,	0,1,	0,0,1, -1,0,0,   1, 1, 1, 1},// 3

		// Front
		{50.0f,50.0f,-50.0f,	1,0,	0,0,-1, 1,0,0,   1, 1, 1, 1},// 4
		{-50.0f,50.0f,-50.0f,	0,0,	0,0,-1, 1,0,0,   1, 1, 1, 1},// 5
		{50.0f,-50.0f,-50.0f,	1,1,	0,0,-1, 1,0,0,   1, 1, 1, 1},// 6
		{-50.0f,-50.0f,-50.0f,	0,1,	0,0,-1, 1,0,0,   1, 1, 1, 1},// 7

		// Bottom
		{50.0f,-50.0f,50.0f,	1,0,	0,-1,0, 1,0,0,     1, 1, 1, 1},// 8
		{50.0f,-50.0f,-50.0f,	0,0,	0,-1,0, 1,0,0,     1, 1, 1, 1},// 9
		{-50.0f,-50.0f,50.0f,	1,1,	0,-1,0, 1,0,0,     1, 1, 1, 1},// 10 
		{-50.0f,-50.0f,-50.0f,	0,1,	0,-1,0, 1,0,0,     1, 1, 1, 1},// 11

		// Top
		{50.0f,50.0f,50.0f,		1,0,	0,1,0, -1,0,0,    1, 1, 1, 1},// 12
		{-50.0f,50.0f,50.0f,	0,0,	0,1,0, -1,0,0,    1, 1, 1, 1},// 13
		{50.0f,50.0f,-50.0f,	1,1,	0,1,0, -1,0,0,    1, 1, 1, 1},// 14
		{-50.0f,50.0f,-50.0f,	0,1,	0,1,0, -1,0,0,    1, 1, 1, 1},// 15

		// Left
		{-50.0f,50.0f,50.0f,	1,0,	-1,0,0, 0,0,1,    1, 1, 1, 1},// 16
		{-50.0f,50.0f,-50.0f,	0,0,	-1,0,0, 0,0,1,    1, 1, 1, 1},// 17
		{-50.0f,-50.0f,50.0f,	1,1,	-1,0,0, 0,0,1,    1, 1, 1, 1},// 18
		{-50.0f,-50.0f,-50.0f,	0,1,	-1,0,0, 0,0,1,    1, 1, 1, 1},// 19

		// Right
		{50.0f,50.0f,50.0f,		1,0,	1,0,0, 0,0,-1,     1, 1, 1, 1},// 20
		{50.0f,-50.0f,50.0f,	0,0,	1,0,0, 0,0,-1,     1, 1, 1, 1},// 21
		{50.0f,50.0f,-50.0f,	1,1,	1,0,0, 0,0,-1,     1, 1, 1, 1},// 22
		{50.0f,-50.0f,-50.0f,	0,1,	1,0,0, 0,0,-1,     1, 1, 1, 1},// 23
	};

	anElement.myMdlIndices = {
		3, 2, 0,	// Back
		0, 1, 3,

		4, 6, 7,	// Front
		7, 5, 4,

		8, 10, 11,	// Bottom
		11, 9, 8,

		12, 14, 15,	// Top
		15, 13, 12,

		19, 18, 16,	// Left
		16, 17, 19,

		23, 22, 20, // Right
		20, 21, 23
	};
}

void Mesh::CreatePlane(Element& anElement)
{
	anElement.myMdlVertices = {
		{500.0f,0.0f,500.0f,		1,0,	0,1,0, -1,0,0,    1, 1, 1, 1},
		{-500.0f,0.0f,500.0f,		0,0,	0,1,0, -1,0,0,    1, 1, 1, 1},
		{500.0f,0.0f,-500.0f,		1,1,	0,1,0, -1,0,0,    1, 1, 1, 1},
		{-500.0f,0.0f,-500.0f,		0,1,	0,1,0, -1,0,0,    1, 1, 1, 1}
	};

	anElement.myMdlIndices = {
		0, 2, 3,
		3, 1, 0
	};
}

void Mesh::CreateSphere(Element& anElement)
{
	/*const int numSegments = 50;
	const int numRings = 50;
	const float radius = 50.0f;

	for (int i = 0; i <= numRings; i++)
	{
		float v = static_cast<float>(i) / static_cast<float>(numRings);
		float phi = v * Pi;

		for (int j = 0; j <= numSegments; j++)
		{
			float u = static_cast<float>(j) / static_cast<float>(numSegments);
			float theta = u * (Pi * 2);

			float x = radius * cosf(theta) * sinf(phi);
			float y = radius * cosf(phi);
			float z = radius * sinf(theta) * sinf(phi);

			Vertex vertex(x, y, z, 0.5f, 0.5f, 0.5f);
			anElement.myMdlVertices.push_back(vertex);
		}
	}

	for (int i = 0; i < numRings; i++)
	{
		for (int j = 0; j < numSegments; j++)
		{
			int i0 = i * (numSegments + 1) + j;
			int i1 = i0 + 1;
			int i2 = (i + 1) * (numSegments + 1) + j;
			int i3 = i2 + 1;

			anElement.myMdlIndices.push_back(i0);
			anElement.myMdlIndices.push_back(i2);
			anElement.myMdlIndices.push_back(i1);

			anElement.myMdlIndices.push_back(i1);
			anElement.myMdlIndices.push_back(i2);
			anElement.myMdlIndices.push_back(i3);
		}
	}*/
}

void Mesh::CreatePyramid(Element& anElement)
{
	/*anElement.myMdlVertices = {

{0, 50.0f, 0, 1, 0, 0 ,1},

{50.0f, -50.0f, 50.0f, 0, 0, 1, 1},

{50.0f, -50.0f, -50.0f, 0, 1, 1, 1},

{-50.0f, -50.0f, 50.0f, 1, 1, 1, 1},

{-50.0f, -50.0f, -50.0f, 0, 0, 0, 1}
	};

	anElement.myMdlIndices = {
		1, 2, 0,
		2, 4, 0,
		4, 3, 0,
		3, 1, 0,
		3, 4, 2,
		2, 1, 3
	};*/
}

CommonUtilities::AABB3D<float>& Mesh::GetAABB()
{
	CommonUtilities::AABB3D<float> temp;
	CommonUtilities::Vector3f min = { 0,0,0 };
	CommonUtilities::Vector3f max = { 0,0,0 };

	for (int i = 0; i < myElements.size(); i++)
	{
		if (myElements[i].myAABB.GetMin().x < min.x)
		{
			min.x = myElements[i].myAABB.GetMin().x;
		}
		if (myElements[i].myAABB.GetMin().y < min.y)
		{
			min.y = myElements[i].myAABB.GetMin().y;
		}
		if (myElements[i].myAABB.GetMin().z < min.z)
		{
			min.z = myElements[i].myAABB.GetMin().z;
		}

		if (myElements[i].myAABB.GetMax().x > max.x)
		{
			max.x = myElements[i].myAABB.GetMax().x;
		}
		if (myElements[i].myAABB.GetMax().y > max.y)
		{
			max.y = myElements[i].myAABB.GetMax().y;
		}
		if (myElements[i].myAABB.GetMax().z > max.z)
		{
			max.z = myElements[i].myAABB.GetMax().z;
		}
	}

	temp.InitWithMinAndMax(min, max);
	return temp;
}

Mesh::Mesh(Shape aShape)
{
	Element tempElement;

	switch (aShape)
	{
	case Cube:
		CreateCube(tempElement);
		tempElement.myAABB.InitWithMinAndMax(
			CommonUtilities::Vector3f(tempElement.myMdlVertices[11].myPosition.x, tempElement.myMdlVertices[11].myPosition.y, tempElement.myMdlVertices[11].myPosition.z),
			CommonUtilities::Vector3f(tempElement.myMdlVertices[12].myPosition.x, tempElement.myMdlVertices[12].myPosition.y, tempElement.myMdlVertices[12].myPosition.z));
		break;
	case Sphere:
		CreateSphere(tempElement);
		break;
	case Pyramid:
		CreatePyramid(tempElement);
		break;
	case Plane:
		CreatePlane(tempElement);
		tempElement.myAABB.InitWithMinAndMax(
			CommonUtilities::Vector3f(tempElement.myMdlVertices[3].myPosition.x, tempElement.myMdlVertices[3].myPosition.y, tempElement.myMdlVertices[3].myPosition.z),
			CommonUtilities::Vector3f(tempElement.myMdlVertices[0].myPosition.x, tempElement.myMdlVertices[0].myPosition.y, tempElement.myMdlVertices[0].myPosition.z));
		break;
	default:
		break;
	}
	RHI::CreateVertexBuffer(tempElement.myVertexBuffer, tempElement.myMdlVertices);
	RHI::CreateIndexBuffer(tempElement.myIndexBuffer, tempElement.myMdlIndices);
	tempElement.myNumVertecies = AsUINT(tempElement.myMdlVertices.size());
	tempElement.myNumIndices = AsUINT(tempElement.myMdlIndices.size());
	tempElement.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	tempElement.myStride = sizeof(Vertex);

	myElements.push_back(tempElement);
}

Mesh::Mesh(Shape aShape, std::vector<CommonUtilities::Transform> someTransforms)
	: Mesh(aShape)
{
	std::vector<CommonUtilities::Matrix4f> instances;
	instances.reserve(someTransforms.size());
	for (size_t i = 0; i < someTransforms.size(); i++)
	{
		instances.push_back(someTransforms[i].GetMatrix());
	}
	
	myInstanceCount = someTransforms.size();
	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(CommonUtilities::Matrix4f) * instances.size());
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = instances.data();

	RHI::Device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, myInstanceBuffer.GetAddressOf());
}


Mesh::Mesh(const TGA::FBX::Mesh& aTGAMesh)
{
	for (int i = 0; i < aTGAMesh.Elements.size(); i++)
	{
		Element tempElement;

		for (int j = 0; j < aTGAMesh.Elements[i].Vertices.size(); j++)
		{
			Vertex temp(aTGAMesh.Elements[i].Vertices[j]);
			tempElement.myMdlVertices.push_back(temp);
		}

		for (unsigned int k = 0; k < aTGAMesh.Elements[i].Indices.size(); k++)
		{
			tempElement.myMdlIndices.push_back(aTGAMesh.Elements[i].Indices[k]);
		}

		RHI::CreateVertexBuffer(tempElement.myVertexBuffer, tempElement.myMdlVertices);
		RHI::CreateIndexBuffer(tempElement.myIndexBuffer, tempElement.myMdlIndices);
		tempElement.myNumVertecies = AsUINT(tempElement.myMdlVertices.size());
		tempElement.myNumIndices = AsUINT(tempElement.myMdlIndices.size());
		tempElement.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		tempElement.myStride = sizeof(Vertex);
		tempElement.myAABB.InitWithMinAndMax(
			CommonUtilities::Vector3f(aTGAMesh.BoxBounds.Min[0], aTGAMesh.BoxBounds.Min[1], aTGAMesh.BoxBounds.Min[2]),
			CommonUtilities::Vector3f(aTGAMesh.BoxBounds.Max[0], aTGAMesh.BoxBounds.Max[1], aTGAMesh.BoxBounds.Max[2]));

		myElements.push_back(tempElement);
	}

}

Mesh::Mesh(const TGA::FBX::Mesh& aTGAMesh, const Skeleton& aSkeleton)
{
	for (int i = 0; i < aTGAMesh.Elements.size(); i++)
	{
		Element tempElement;

		for (int j = 0; j < aTGAMesh.Elements[i].Vertices.size(); j++)
		{
			Vertex temp(aTGAMesh.Elements[i].Vertices[j]);
			tempElement.myMdlVertices.push_back(temp);
		}

		for (unsigned int k = 0; k < aTGAMesh.Elements[i].Indices.size(); k++)
		{
			tempElement.myMdlIndices.push_back(aTGAMesh.Elements[i].Indices[k]);
		}

		RHI::CreateVertexBuffer(tempElement.myVertexBuffer, tempElement.myMdlVertices);
		RHI::CreateIndexBuffer(tempElement.myIndexBuffer, tempElement.myMdlIndices);
		tempElement.myNumVertecies = AsUINT(tempElement.myMdlVertices.size());
		tempElement.myNumIndices = AsUINT(tempElement.myMdlIndices.size());
		tempElement.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		tempElement.myStride = sizeof(Vertex);
		tempElement.myAABB.InitWithMinAndMax(
			CommonUtilities::Vector3f(aTGAMesh.BoxBounds.Min[0], aTGAMesh.BoxBounds.Min[1], aTGAMesh.BoxBounds.Min[2]),
			CommonUtilities::Vector3f(aTGAMesh.BoxBounds.Max[0], aTGAMesh.BoxBounds.Max[1], aTGAMesh.BoxBounds.Max[2]));

		myElements.push_back(tempElement);
	}
	mySkeleton = aSkeleton;
}

bool Mesh::HasInstances() const
{
	return myInstanceBuffer;
}

const ComPtr<ID3D11Buffer> Mesh::GetInstanceBuffer() const
{
	return myInstanceBuffer;
}

void Mesh::SetMaterialsToElements(std::shared_ptr<Material>& aMaterial)
{
	for (int i = 0; i < myElements.size(); i++)
	{
		myElements[i].myMaterial = aMaterial;
	}
}

const unsigned Mesh::GetInstanceCount() const
{
	return myInstanceCount;
}
