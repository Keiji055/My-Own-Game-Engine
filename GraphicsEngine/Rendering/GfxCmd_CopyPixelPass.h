#pragma once
#include <GraphicsCommand.h>
#include <Shaders\Include\CopyPS.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"

class GfxCmd_CopyPixelPass : public GraphicsCommand
{
public:
	GfxCmd_CopyPixelPass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget);
	void Execute();

private:
	std::shared_ptr<Texture> mySourceTexture;
	std::shared_ptr<Texture> myTargetTexture;
};