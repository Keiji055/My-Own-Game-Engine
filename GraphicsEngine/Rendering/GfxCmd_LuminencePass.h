#pragma once
#include <GraphicsCommand.h>
#include <Shaders\Include\LuminencePS.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"

class GfxCmd_LuminencePass : public GraphicsCommand
{
public:
	GfxCmd_LuminencePass(const std::shared_ptr<Texture>& aSceneBuffer, const std::shared_ptr<Texture>& aIntermediateTexture);
	void Execute();

private:
	std::shared_ptr<Texture> mySceneBufferTexture;
	std::shared_ptr<Texture> myIntermediateTexture;
};