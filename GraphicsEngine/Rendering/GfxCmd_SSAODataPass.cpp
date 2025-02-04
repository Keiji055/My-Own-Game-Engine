#include "GraphicsEngine.pch.h"
#include "GfxCmd_SSAODataPass.h"
#include "../InterOp/RHI.h"

GfxCmd_SSAODataPass::GfxCmd_SSAODataPass(const std::weak_ptr<Texture>& anEmptyTexture, const std::weak_ptr<Texture>& aBlueNoiseTexture)
{
	myBlueNoiseTexture = aBlueNoiseTexture;
	myEmptyTexture = anEmptyTexture;
}

void GfxCmd_SSAODataPass::Execute()
{
	RHI::SetPixelShader(GraphicsEngine::Get().GetSSAOPixelShader()); 
	RHI::SetVertexShader(GraphicsEngine::Get().GetScreenSpaceQuadVertexShader());

	RHI::ClearRenderTarget(myEmptyTexture.lock().get());
	
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 8, myBlueNoiseTexture.lock().get());
	RHI::SetRenderTarget(myEmptyTexture.lock().get(), nullptr);

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);

	RHI::Draw(4);

	RHI::SetRenderTarget(nullptr, nullptr);
	RHI::SetTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 9, myEmptyTexture.lock().get());
	RHI::RemoveTextureResource(PIPELINE_STAGE_PIXEL_SHADER, 8);
}
