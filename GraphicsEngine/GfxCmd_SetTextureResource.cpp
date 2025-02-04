#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetTextureResource.h"

GfxCmd_SetTextureResource::GfxCmd_SetTextureResource(const std::shared_ptr<Texture>& aTexture, const unsigned int& aTextureSlot)
{
	myTexture = aTexture;
	myTextureSlot = aTextureSlot;
}

void GfxCmd_SetTextureResource::Execute()
{
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, myTextureSlot, myTexture.get());
}
