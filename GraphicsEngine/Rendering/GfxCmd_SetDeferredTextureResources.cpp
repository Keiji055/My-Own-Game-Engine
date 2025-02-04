#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetDeferredTextureResources.h"

GfxCmd_SetDeferredTextureResources::GfxCmd_SetDeferredTextureResources(const std::vector<std::shared_ptr<Texture>>& someTextures, const unsigned int& aStartingTextureSlot)
{
	myTextures = someTextures;
	myStartingTextureSlot = aStartingTextureSlot;
}

void GfxCmd_SetDeferredTextureResources::Execute()
{
	RHI::SetTextureResources(PIPELINE_STAGE_PIXEL_SHADER, myStartingTextureSlot, myTextures);
}
