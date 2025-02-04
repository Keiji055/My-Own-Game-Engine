#pragma once
#include "../GraphicsEngine.h"
#include <GraphicsCommand.h>

class GfxCmd_ClearDepthStencil : public GraphicsCommand
{
	std::shared_ptr<Texture> myTexture;
public:
	GfxCmd_ClearDepthStencil(const std::shared_ptr<Texture>& aTexture);
	void Execute() override;
};
