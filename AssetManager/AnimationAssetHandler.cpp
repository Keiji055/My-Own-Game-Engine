#include "AssetManager.pch.h"
#include "AnimationAssetHandler.h"

bool AnimationAssetHandler::LoadAnimationFromFBX(const std::string& aPath, std::shared_ptr<Animation>& inoutAsset) const
{
	TGA::FBX::Animation tgaAnimation;

	if (TGA::FBX::Importer::LoadAnimationA(aPath, tgaAnimation))
	{
		inoutAsset = std::make_shared<Animation>(tgaAnimation);
		return false;
	}
	else
	{
		return false;
	}
}

AnimationAssetHandler& AnimationAssetHandler::GetInstance()
{
	static AnimationAssetHandler instance;
	return instance;
}

std::shared_ptr<Animation> AnimationAssetHandler::TryGetAsset(const std::string& aPath)
{
	if (myMeshMap.count(aPath) > 0)
	{
		return myMeshMap.at(aPath);
	}
	else
	{
		std::shared_ptr<Animation> temp;
		LoadAnimationFromFBX(aPath, temp);

		myMeshMap.emplace(aPath, temp);
		return temp;
	}
}
