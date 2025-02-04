#pragma once
#include "GraphicsCommand.h"
#include <memory>
#include "../Rendering/ParticleEmitter.h"
class GfxCmd_RenderParticles : public GraphicsCommand
{
public:
	GfxCmd_RenderParticles(const std::shared_ptr<ParticleEmitter>& aParticleEmitter);
	void Execute() override;
private:
	std::shared_ptr<ParticleEmitter> myParticleEmitter;
};