#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetRenderTarget.h"

GfxCmd_SetRenderTarget::GfxCmd_SetRenderTarget(const std::shared_ptr<Texture>& aTarget, const std::shared_ptr<Texture>& aDepthStencil)
{
	myTarget = aTarget;
	myDepth = aDepthStencil;
}

void GfxCmd_SetRenderTarget::Execute()
{
	RHI::SetRenderTarget(myTarget.get(), myDepth.get());
}
