#pragma once
#include<../GraphicsEngine/Rendering/Texture.h>
#include <../GraphicsEngine/InterOp/RHI.h>
#include <unordered_map>

class TextureAssetHandler
{
public:

	bool LoadTextureFromFile(const std::wstring& aPath, Texture* aInOutAsset) const;
	static TextureAssetHandler& GetInstance();
	std::weak_ptr<Texture> TryGetAsset(const std::wstring& aPath);

private:
	std::unordered_map <std::wstring, std::shared_ptr<Texture>> myTextureMap;
};