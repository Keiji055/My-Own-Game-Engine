#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>
#include "../GraphicsEngine/Rendering/ParticleEmitterTemplate.h"
#include "../../ThirdParty/rapidjson/rapidjson.h"
#include "../../ThirdParty/rapidjson/filereadstream.h"
#include "../../ThirdParty/rapidjson/document.h"
#include "../../ThirdParty/rapidjson/writer.h"
#include "../../ThirdParty/rapidjson/stringbuffer.h"

class ParticleAssetHandler
{
public:
	static ParticleAssetHandler& GetInstance();
	std::shared_ptr<EmitterSettingsData> TryGetAsset(const std::wstring& aPath);
	bool LoadParticleSystemFromFile(const std::wstring& aPath, EmitterSettingsData* aInOutAsset) const;
private:
	std::unordered_map <std::wstring, std::shared_ptr<EmitterSettingsData>> myParticleSystemMap;
};