#pragma once
#include "GraphicsCommand.h"
#include <InterOp/RHI.h>
#include <Rendering/Texture.h>
#include <memory>

class GfxCmd_SetDeferredTextureResources : public GraphicsCommand
{
	std::vector<std::shared_ptr<Texture>> myTextures;
	int myStartingTextureSlot;
public:
	GfxCmd_SetDeferredTextureResources(const std::vector<std::shared_ptr<Texture>>& someTextures, const unsigned int& aStartingTextureSlot);
	void Execute() override;
};