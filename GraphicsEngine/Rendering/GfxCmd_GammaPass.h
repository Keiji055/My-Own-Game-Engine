#pragma once
#include <GraphicsCommand.h>
#include <Shaders\Include\GammaPS.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"

class GfxCmd_GammaPass : public GraphicsCommand
{
public:
	GfxCmd_GammaPass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget);
	void Execute();

private:
	std::shared_ptr<Texture> mySourceTexture;
	std::shared_ptr<Texture> myTargetTexture;
};