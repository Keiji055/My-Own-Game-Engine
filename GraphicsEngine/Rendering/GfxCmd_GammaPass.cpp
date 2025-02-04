#include "GraphicsEngine.pch.h"
#include "GfxCmd_GammaPass.h"

GfxCmd_GammaPass::GfxCmd_GammaPass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget)
{
	mySourceTexture = aTextureSource;
	myTargetTexture = aTextureTarget;
}

void GfxCmd_GammaPass::Execute()
{
	RHI::SetPixelShader(GraphicsEngine::Get().GetGammaPixelShader());

	RHI::SetRenderTarget(myTargetTexture.get(), nullptr);
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, mySourceTexture.get());

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);

	RHI::Draw(4);

	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0);
}
