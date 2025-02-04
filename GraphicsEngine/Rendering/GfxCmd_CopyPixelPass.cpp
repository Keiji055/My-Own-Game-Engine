#include "GraphicsEngine.pch.h"
#include "GfxCmd_CopyPixelPass.h"

GfxCmd_CopyPixelPass::GfxCmd_CopyPixelPass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget)
{
	mySourceTexture = aTextureSource;
	myTargetTexture = aTextureTarget;
}

void GfxCmd_CopyPixelPass::Execute()
{
	RHI::SetPixelShader(GraphicsEngine::Get().GetCopyPixelShader());

	RHI::SetRenderTarget(myTargetTexture.get(), nullptr);
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1, mySourceTexture.get());

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);

	RHI::Draw(4);

	RHI::SetRenderTarget(nullptr, nullptr);
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1);
}
