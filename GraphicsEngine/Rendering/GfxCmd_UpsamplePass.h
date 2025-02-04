#pragma once
#include <GraphicsCommand.h>
#include <Shaders\Include\UpsamplingPS.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"

class GfxCmd_UpsamplePass : public GraphicsCommand
{
public:
	GfxCmd_UpsamplePass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget);
	void Execute();

private:
	std::shared_ptr<Texture> mySourceTexture;
	std::shared_ptr<Texture> myTargetTexture;
};