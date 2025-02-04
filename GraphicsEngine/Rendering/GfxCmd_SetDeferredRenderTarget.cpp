#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetDeferredRenderTarget.h"

GfxCmd_SetDeferredRenderTarget::GfxCmd_SetDeferredRenderTarget(const GBufferData& someTargets, const std::shared_ptr<Texture>& aDepthStencil)
{
	myTargets.push_back(someTargets.Albedo);
	myTargets.push_back(someTargets.PixelNormal);
	myTargets.push_back(someTargets.Material);
	myTargets.push_back(someTargets.VertexNormal);
	myTargets.push_back(someTargets.WorldPosition);
	myTargets.push_back(someTargets.FXTexture);
	myDepth = aDepthStencil;
}

void GfxCmd_SetDeferredRenderTarget::Execute()
{
	RHI::SetRenderTargets(myTargets, myDepth);
	myTargets.clear();
}
