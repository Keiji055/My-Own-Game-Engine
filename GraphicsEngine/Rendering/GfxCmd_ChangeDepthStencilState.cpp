#include "GraphicsEngine.pch.h"
#include "GfxCmd_ChangeDepthStencilState.h"

GfxCmd_ChangeDepthStencilState::GfxCmd_ChangeDepthStencilState(DepthStencilState aState)
{
	myState = aState;
}

void GfxCmd_ChangeDepthStencilState::Execute()
{
	RHI::Context->OMSetDepthStencilState(GraphicsEngine::Get().GetDepthStencilStates()[myState], myState);
}
