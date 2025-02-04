#pragma once
#include <GraphicsCommand.h>
#include <Shaders\Include\BloomPS.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"

class GfxCmd_BloomPass : public GraphicsCommand
{
public:
	GfxCmd_BloomPass(const std::shared_ptr<Texture>& aTextureResource1, const std::shared_ptr<Texture>& aTextureResource2);
	void Execute();

private:
	std::shared_ptr<Texture> myResourceTexture1;
	std::shared_ptr<Texture> myResourceTexture2;
};