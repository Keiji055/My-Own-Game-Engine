#pragma once
#include "GraphicsCommand.h"
#include <InterOp/RHI.h>
#include <Rendering/Texture.h>
#include <memory>

class GfxCmd_SetTextureResource : public GraphicsCommand
{
	std::shared_ptr<Texture> myTexture;
	int myTextureSlot;
public:
	GfxCmd_SetTextureResource(const std::shared_ptr<Texture>& aTexture, const unsigned int& aTextureSlot);
	void Execute() override;
};