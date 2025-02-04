#include "GraphicsEngine.pch.h"
#include "GfxCmd_EnvironmentLightPass.h"

GfxCmd_EnvironmentLightPass::GfxCmd_EnvironmentLightPass()
{

}

void GfxCmd_EnvironmentLightPass::Execute()
{
	RHI::SetPixelShader(GraphicsEngine::Get().GetEnvironmentPixelShader());
	RHI::SetVertexShader(GraphicsEngine::Get().GetScreenSpaceQuadVertexShader());

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);

	RHI::Draw(4);
}
