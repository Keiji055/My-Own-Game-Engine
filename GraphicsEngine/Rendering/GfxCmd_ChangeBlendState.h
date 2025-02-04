#pragma once
#include "../GraphicsEngine.h"
#include <GraphicsCommand.h>

class GfxCmd_ChangeBlendState : public GraphicsCommand
{
	BlendState myBlendState;
public:
	GfxCmd_ChangeBlendState(BlendState& aBlendState);
	void Execute() override;
};
