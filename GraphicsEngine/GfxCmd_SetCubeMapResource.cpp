#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetCubeMapResource.h"

GfxCmd_SetCubeMapResource::GfxCmd_SetCubeMapResource(const std::weak_ptr<Texture>& aTexture)
{
	myCubeMap = aTexture;
}

void GfxCmd_SetCubeMapResource::Execute()
{
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 50, myCubeMap.lock().get());
}
