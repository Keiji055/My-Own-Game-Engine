#pragma once
#include "../GraphicsCommand.h"

class GfxCmd_ToneMapPass : public GraphicsCommand
{
private:
	std::shared_ptr<Texture> mySourceTexture;
	std::shared_ptr<Texture> myTargetTexture;

public:
	GfxCmd_ToneMapPass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget);
	void Execute() override;
};