#include "GraphicsEngine.pch.h"
#include "GraphicsCommand.h"

FrameBuffer& GraphicsCommand::GetFrameBuffer()
{
	return GraphicsEngine::Get().myFrameBuffer;
}

ObjectBuffer& GraphicsCommand::GetObjectBuffer()
{
	return GraphicsEngine::Get().myObjectBuffer;
}

LightBuffer& GraphicsCommand::GetLightBuffer()
{
	return GraphicsEngine::Get().myLightBuffer;
}

MaterialBuffer& GraphicsCommand::GetMaterialBuffer()
{
	return GraphicsEngine::Get().myMaterialBuffer;
}

PointLightBuffer& GraphicsCommand::GetPointLightBuffer()
{
	return GraphicsEngine::Get().myPointLightBuffer;
}

SpotLightBuffer& GraphicsCommand::GetSpotLightBuffer()
{
	return GraphicsEngine::Get().mySpotLightBuffer;
}