#include "AssetManager.pch.h"
#include "ParticleAssetHandler.h"

ParticleAssetHandler& ParticleAssetHandler::GetInstance()
{
	static ParticleAssetHandler instance;
	return instance;
}

std::shared_ptr<EmitterSettingsData> ParticleAssetHandler::TryGetAsset(const std::wstring& aPath)
{
	if (myParticleSystemMap.count(aPath) > 0)
	{
		return myParticleSystemMap.at(aPath);
	}
	else
	{
		std::shared_ptr<EmitterSettingsData> temp = std::make_shared<EmitterSettingsData>();
		LoadParticleSystemFromFile(aPath, temp.get());
		myParticleSystemMap.emplace(aPath, temp);
		return temp;
	}
}

bool ParticleAssetHandler::LoadParticleSystemFromFile(const std::wstring& aPath, EmitterSettingsData* aInOutAsset) const
{
	FILE* fp;
	errno_t err = fopen_s(&fp, "../../Assets/Effects/ParticleSystems/ParticleSystemEmitter_1.json", "rb");

	if (err == 0)
	{
		char readBuffer[10000] = {};
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		rapidjson::Document document;
		document.ParseStream(is);
		aInOutAsset->GravityModifier = document["GravityModifier"].GetFloat();
		aInOutAsset->SpawnRate = document["SpawnRate"].GetFloat();
		aInOutAsset->SpawnAngle = document["SpawnAngle"].GetFloat();
		aInOutAsset->LifeTime = document["LifeTime"].GetFloat();
		aInOutAsset->StartVelocity.x = document["StartVelocity"]["X"].GetFloat();
		aInOutAsset->StartVelocity.y = document["StartVelocity"]["Y"].GetFloat();
		aInOutAsset->EndVelocity.x = document["EndVelocity"]["X"].GetFloat();
		aInOutAsset->EndVelocity.y = document["EndVelocity"]["Y"].GetFloat();
		aInOutAsset->StartSize = document["StartSize"].GetFloat();
		aInOutAsset->EndSize = document["EndSize"].GetFloat();
		aInOutAsset->StartColor.x = document["StartColor"]["R"].GetFloat();
		aInOutAsset->StartColor.y = document["StartColor"]["G"].GetFloat();
		aInOutAsset->StartColor.z = document["StartColor"]["B"].GetFloat();
		aInOutAsset->StartColor.w = document["StartColor"]["A"].GetFloat();
		aInOutAsset->EndColor.x = document["EndColor"]["R"].GetFloat();
		aInOutAsset->EndColor.y = document["EndColor"]["G"].GetFloat();
		aInOutAsset->EndColor.z = document["EndColor"]["B"].GetFloat();
		aInOutAsset->EndColor.w = document["EndColor"]["A"].GetFloat();
		
		fclose(fp);
		
		return true;
	}
	else
	{
		return false;
		std::cout << "error json file could not be read!" << std::endl;
	}

}
