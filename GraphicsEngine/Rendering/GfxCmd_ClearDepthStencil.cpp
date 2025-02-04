#include "GraphicsEngine.pch.h"
#include "GfxCmd_ClearDepthStencil.h"

GfxCmd_ClearDepthStencil::GfxCmd_ClearDepthStencil(const std::shared_ptr<Texture>& aTexture)
{
	myTexture = aTexture;
}

void GfxCmd_ClearDepthStencil::Execute()
{
	RHI::ClearDepthStencil(myTexture.get());
}
