#include "GraphicsEngine.pch.h"
#include "GfxCmd_ChangeBlendState.h"

GfxCmd_ChangeBlendState::GfxCmd_ChangeBlendState(BlendState& aBlendState)
{
	myBlendState = aBlendState;
}

void GfxCmd_ChangeBlendState::Execute()
{
	switch (myBlendState)
	{
	case BlendState::eAlpha:
		GraphicsEngine::Get().SetBlendStateToAlpha();
		break;
	case BlendState::eAddative:
		GraphicsEngine::Get().SetBlendStateToAddative();
		break;
	case BlendState::eNone:
		GraphicsEngine::Get().SetBlendStateToNone();
		break;
	default:
		break;
	}
}
