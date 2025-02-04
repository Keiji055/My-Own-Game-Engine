#pragma once
#include "../GraphicsCommand.h"
#include "Texture.h"
#include "../InterOp/RHI.h"

class GfxCmd_SetRenderTarget : public GraphicsCommand
{
	std::shared_ptr<Texture> myTarget;
	std::shared_ptr<Texture> myDepth;

public:
	GfxCmd_SetRenderTarget(const std::shared_ptr<Texture>& aTarget, const std::shared_ptr<Texture>& aDepthStencil);
	void Execute() override;
};