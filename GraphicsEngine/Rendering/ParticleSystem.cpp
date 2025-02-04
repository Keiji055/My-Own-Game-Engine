#include "GraphicsEngine.pch.h"
#include "ParticleSystem.h"

void ParticleSystem::Init(const Vector3f& aPosition, EmitterSettingsData& someSettings, ParticleEmitter& anEmitter)
{

}

void ParticleSystem::Update(float aDeltaTime)
{
	for (ParticleSystemEmitter& emitter : myEmitters)
	{
		emitter.Emitter.Update(aDeltaTime);
	}
}

void ParticleSystem::SetEmitter(const ParticleSystemEmitter& anEmitter)
{
	myEmitters.push_back(anEmitter);
}
