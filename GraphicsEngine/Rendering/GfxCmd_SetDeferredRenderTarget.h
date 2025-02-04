#pragma once
#include "../GraphicsCommand.h"
#include "Texture.h"
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"

class GfxCmd_SetDeferredRenderTarget : public GraphicsCommand
{
	std::vector<std::shared_ptr<Texture>> myTargets;
	std::shared_ptr<Texture> myDepth;

public:
	GfxCmd_SetDeferredRenderTarget(const GBufferData& someTargets, const std::shared_ptr<Texture>& aDepthStencil);
	void Execute() override;
};