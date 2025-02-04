#pragma once
#include "GraphicsCommand.h"
#include <memory>
class GfxCmd_ChangeDepthStencilState : public GraphicsCommand
{
public:
	GfxCmd_ChangeDepthStencilState(DepthStencilState aState);
	void Execute() override;
private:
	DepthStencilState myState;
};