#include "AssetManager.pch.h"
#include "ModelAssetHandler.h"

bool ModelAssetHandler::InitUnitCube()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(Shape::Cube);
	myMeshMap.emplace("Cube", mesh);
	mesh = std::make_shared<Mesh>(Shape::Plane);
	myMeshMap.emplace("Plane", mesh);
	return true;
}

bool ModelAssetHandler::LoadMeshFromFBX(const std::string& aPath, std::shared_ptr<Mesh>& inoutAsset) const
{
	TGA::FBX::Mesh tgaMesh;

	if (TGA::FBX::Importer::LoadMeshA(aPath, tgaMesh))
	{
		Mesh::Skeleton meshSkeleton;

		const bool hasSkeleton = tgaMesh.Skeleton.GetRoot();

		if (hasSkeleton)
		{
			meshSkeleton.name = tgaMesh.Skeleton.Name;
			for (int index = 0; index < tgaMesh.Skeleton.Bones.size(); index++)
			{
				meshSkeleton.boneNameToIndex.emplace(tgaMesh.Skeleton.Bones[index].Name, index);
				Mesh::Skeleton::Bone tempBone;
				tempBone.name = tgaMesh.Skeleton.Bones[index].Name;
				tempBone.parentId = tgaMesh.Skeleton.Bones[index].ParentIdx;
				memcpy_s(&tempBone.bindPoseInverse, sizeof(CommonUtilities::Matrix4x4<float>), &tgaMesh.Skeleton.Bones[index].BindPoseInverse, sizeof(float) * 16);
				
				for (int childrenIndex = 0; childrenIndex < tgaMesh.Skeleton.Bones[index].Children.size(); childrenIndex++)
				{
					tempBone.children.push_back(tgaMesh.Skeleton.Bones[index].Children[childrenIndex]);
				}

				meshSkeleton.bones.push_back(tempBone);
			}	
			inoutAsset = std::make_shared<Mesh>(tgaMesh, meshSkeleton);
			return true;
		}

		inoutAsset = std::make_shared<Mesh>(tgaMesh);
		return true;
	}
	else
	{
		return false;
	}
}

ModelAssetHandler& ModelAssetHandler::GetInstance()
{
	static ModelAssetHandler instance;
	return instance;
}

std::shared_ptr<Mesh> ModelAssetHandler::TryGetAsset(const std::string& aPath)
{
	if (myMeshMap.count(aPath) > 0)
	{
		return myMeshMap.at(aPath);
	}
	else
	{
		std::shared_ptr<Mesh> temp;
		LoadMeshFromFBX(aPath, temp);

		myMeshMap.emplace(aPath, temp);
		return temp;
	}
}

