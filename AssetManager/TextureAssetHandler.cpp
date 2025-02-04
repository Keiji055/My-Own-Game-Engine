#include "AssetManager.pch.h"
#include "TextureAssetHandler.h"

bool TextureAssetHandler::LoadTextureFromFile(const std::wstring& aPath, Texture* aInOutAsset) const
{
	return RHI::LoadTexture(aInOutAsset, aPath);
}

TextureAssetHandler& TextureAssetHandler::GetInstance()
{
	static TextureAssetHandler instance;
	return instance;
}

std::weak_ptr<Texture> TextureAssetHandler::TryGetAsset(const std::wstring& aPath)
{
	if (myTextureMap.count(aPath) > 0)
	{
		return myTextureMap.at(aPath);
	}
	else
	{
		std::shared_ptr<Texture> temp = std::make_shared<Texture>();
		LoadTextureFromFile(aPath, temp.get());
		myTextureMap.emplace(aPath, temp);
		return temp;
	}
}
