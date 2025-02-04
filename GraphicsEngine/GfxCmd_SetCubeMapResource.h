#pragma once
#include "GraphicsCommand.h"
#include <InterOp/RHI.h>
#include <Rendering/Texture.h>
#include <memory>

class GfxCmd_SetCubeMapResource : public GraphicsCommand
{
	std::weak_ptr<Texture> myCubeMap;
public:
	GfxCmd_SetCubeMapResource(const std::weak_ptr<Texture>& aTexture);
	void Execute() override;
};