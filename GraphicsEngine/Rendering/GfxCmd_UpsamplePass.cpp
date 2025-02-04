#include "GraphicsEngine.pch.h"
#include "GfxCmd_UpsamplePass.h"

GfxCmd_UpsamplePass::GfxCmd_UpsamplePass(const std::shared_ptr<Texture>& aTextureSource, const std::shared_ptr<Texture>& aTextureTarget)
{
	myTargetTexture = aTextureTarget;
	mySourceTexture = aTextureSource;
}

void GfxCmd_UpsamplePass::Execute()
{
	RHI::SetPixelShader(GraphicsEngine::Get().GetUpsamplePixelShader());

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
}
