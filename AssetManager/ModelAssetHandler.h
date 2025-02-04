#pragma once
#include <string>
#include <TGAFbx.h>
#include <GraphicsEngine/Objects/Mesh.h>
#include <memory>

class ModelAssetHandler 
{
public:
	bool InitUnitCube();

	bool LoadMeshFromFBX(const std::string& aPath, std::shared_ptr<Mesh>& inoutAsset) const;

	bool Initialize() 
	{
		TGA::FBX::Importer::InitImporter();
		return InitUnitCube();
	}

	static ModelAssetHandler& GetInstance();
	std::shared_ptr<Mesh> TryGetAsset(const std::string& aPath);
	
private:
	std::unordered_map < std::string,std::shared_ptr<Mesh>> myMeshMap;
};
