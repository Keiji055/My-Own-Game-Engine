#pragma once
#include <GraphicsCommand.h>
#include <Shaders\Include\GaussianBlurPS.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"

class GfxCmd_GaussianBlurPass : public GraphicsCommand
{
public:
	GfxCmd_GaussianBlurPass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget);
	void Execute();

private:
	std::shared_ptr<Texture> mySourceTexture;
	std::shared_ptr<Texture> myTargetTexture;
};