#pragma once
#include <TGAFbx.h>
#include <GraphicsEngine/Objects/Animation.h>
#include <memory>

class AnimationAssetHandler
{
public:
	bool LoadAnimationFromFBX(const std::string& aPath, std::shared_ptr<Animation>& inoutAsset) const;

	void Initialize()
	{
		TGA::FBX::Importer::InitImporter();
	}

	static AnimationAssetHandler& GetInstance();
	std::shared_ptr<Animation> TryGetAsset(const std::string& aPath);

private:
	std::unordered_map < std::string, std::shared_ptr<Animation>> myMeshMap;

};

