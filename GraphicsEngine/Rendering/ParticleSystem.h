#pragma once
#include "../../AssetManager/ParticleAssetHandler.h"
#include "ParticleSystemTemplate.h"
#include "ParticleEmitter.h"
#include <CommonUtilities/Vector.hpp>

using namespace CommonUtilities;

struct ParticleSystemEmitter
{
	ParticleEmitter Emitter;
	EmitterSettingsData Settings;
	Vector3f Position;
};

class ParticleSystem
{
	friend class ParticleAssetHandler;
	std::vector<ParticleSystemEmitter> myEmitters;

public:
	void Init(const Vector3f& aPosition, EmitterSettingsData& someSettings, ParticleEmitter& anEmitter);
	void Update(float aDeltaTime);
	void SetEmitter(const ParticleSystemEmitter& anEmitter);
	FORCEINLINE const std::vector<ParticleSystemEmitter>& GetEmitters() const { return myEmitters; }

};
