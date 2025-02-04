#include "GraphicsEngine.pch.h"
#include "GfxCmd_LuminencePass.h"

GfxCmd_LuminencePass::GfxCmd_LuminencePass(const std::shared_ptr<Texture>& aSceneBuffer, const std::shared_ptr<Texture>& aIntermediateTexture)
{
	mySceneBufferTexture = aSceneBuffer;
	myIntermediateTexture = aIntermediateTexture;
}

void GfxCmd_LuminencePass::Execute()
{
	RHI::SetPixelShader(GraphicsEngine::Get().GetLuminencePixelShader());

	RHI::SetRenderTarget(myIntermediateTexture.get(), nullptr);
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, mySceneBufferTexture.get());

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);

	RHI::Draw(4);
}
