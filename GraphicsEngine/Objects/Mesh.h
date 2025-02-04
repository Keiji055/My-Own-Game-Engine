#pragma once
#include <wrl.h>
#include <vector>
#include <GraphicsEngine/Rendering/Vertex.h>
#include <CommonUtilities/Transform.h>
#include <CommonUtilities/AABB3D.hpp>
#include "../Rendering/Material.h"
#include <TGAFbx.h>

#define AsUINT(v)(static_cast<unsigned>(v))
using namespace Microsoft::WRL;

struct ID3D11Buffer;

enum Shape
{
	Cube,
	Sphere,
	Pyramid,
	Plane
};

class Mesh
{
public:
	struct Element
	{
		ComPtr<ID3D11Buffer> myVertexBuffer;
		ComPtr<ID3D11Buffer> myIndexBuffer;
		UINT myNumVertecies = 0;
		UINT myNumIndices = 0;
		UINT myPrimitiveTopology = 0;
		UINT myStride = 0;
		std::vector<Vertex> myMdlVertices = {};
		std::vector<unsigned int> myMdlIndices = {};
		std::weak_ptr<Material> myMaterial;
		CommonUtilities::AABB3D<float> myAABB;
	};

	struct Skeleton
	{
		std::string name;

		struct Bone
		{
			CommonUtilities::Matrix4x4<float> bindPoseInverse;
			int parentId = -1;
			std::string name;
			std::vector<unsigned> children;
		};

		std::vector<Bone> bones;
		std::unordered_map<std::string, size_t> boneNameToIndex;

		const Bone* GetRoot() const { if (!bones.empty()) { return &bones[0]; } return nullptr; }
	};

private:
	std::vector<Element> myElements = {};
	Skeleton mySkeleton;
	ComPtr<ID3D11Buffer> myInstanceBuffer;
	unsigned myInstanceCount;

public:
	Mesh(Shape aShape);
	Mesh(Shape aShape, std::vector<CommonUtilities::Transform> someTransforms);
	Mesh(const TGA::FBX::Mesh& aTGAMesh);
	Mesh(const TGA::FBX::Mesh& aTGAMesh, const Skeleton& aSkeleton);
	bool HasInstances() const;
	const ComPtr<ID3D11Buffer> GetInstanceBuffer() const;
	void SetMaterialsToElements(std::shared_ptr<Material>& aMaterial);
	const unsigned GetInstanceCount() const;
	void CreateCube(Element& anElement);
	void CreatePlane(Element& anElement);
	void CreateSphere(Element& anElement);
	void CreatePyramid(Element& anElement);
	CommonUtilities::AABB3D<float>& GetAABB();

	FORCEINLINE const std::vector<Element>& GetElement() const
	{
		return myElements;
	}

	FORCEINLINE const Skeleton& GetSkeleton() const
	{
		return mySkeleton;
	}
};
