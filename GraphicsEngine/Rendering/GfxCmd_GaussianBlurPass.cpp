#include "GraphicsEngine.pch.h"
#include "GfxCmd_GaussianBlurPass.h"

GfxCmd_GaussianBlurPass::GfxCmd_GaussianBlurPass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget)
{
	mySourceTexture = aTextureSource;
	myTargetTexture = aTextureTarget;
}

void GfxCmd_GaussianBlurPass::Execute()
{
	RHI::SetPixelShader(GraphicsEngine::Get().GetGaussianBlurPixelShader());

	RHI::SetRenderTarget(myTargetTexture.get(), nullptr);
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 2, mySourceTexture.get());

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);

	RHI::Draw(4);

	RHI::SetRenderTarget(nullptr, nullptr);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 2);
}
