#pragma once
#include "../GraphicsEngine.h"
#include <GraphicsCommand.h>

class GfxCmd_SSAODataPass : public GraphicsCommand
{
	std::weak_ptr<Texture> myEmptyTexture;
	std::weak_ptr<Texture> myBlueNoiseTexture;
public:
	GfxCmd_SSAODataPass(const std::weak_ptr<Texture>& anEmptyTexture, const std::weak_ptr<Texture>& aBlueNoiseTexture);
	void Execute() override;
};