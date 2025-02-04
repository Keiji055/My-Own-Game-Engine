#include "GraphicsEngine.pch.h"
#include "GfxCmd_BloomPass.h"

GfxCmd_BloomPass::GfxCmd_BloomPass(const std::shared_ptr<Texture>& aTextureResource1, const std::shared_ptr<Texture>& aTextureResource2)
{
	myResourceTexture1 = aTextureResource1;
	myResourceTexture2 = aTextureResource2;
}

void GfxCmd_BloomPass::Execute()
{
	RHI::SetPixelShader(GraphicsEngine::Get().GetBloomPixelShader());

	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 0, myResourceTexture1.get());
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 1, myResourceTexture2.get());

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);

	RHI::Draw(4);
}
