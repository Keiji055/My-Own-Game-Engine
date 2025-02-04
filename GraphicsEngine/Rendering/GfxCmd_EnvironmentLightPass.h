#pragma once
#include <GraphicsCommand.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"
#include <Shaders\Include\EnvironmentPS.h>
#include <Shaders\Include\ScreenSpaceQuad_VS.h>

class GfxCmd_EnvironmentLightPass : public GraphicsCommand
{
public:
	GfxCmd_EnvironmentLightPass();
	void Execute();

private:

};