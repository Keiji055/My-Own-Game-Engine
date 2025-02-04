#include "GraphicsEngine.pch.h"
#include "GfxCmd_PointLightPass.h"

GfxCmd_PointLightPass::GfxCmd_PointLightPass(const CommonUtilities::Vector3f& aColor, const float& anIntensity, 
	const CommonUtilities::Vector3f& aPointLightPosition, const float& aRange)
{
	myPointLightColor = aColor;
	myIntensity = anIntensity;
	myPointLightPosition = aPointLightPosition;
	myRange = aRange;
}

void GfxCmd_PointLightPass::Execute()
{
	PointLightBuffer& pointLightBuffer = GetPointLightBuffer();
	pointLightBuffer.Data.Color = myPointLightColor;
	pointLightBuffer.Data.Intensity = myIntensity;
	pointLightBuffer.Data.Position = myPointLightPosition;
	pointLightBuffer.Data.Range = myRange;

	RHI::UpdateConstantBufferData(pointLightBuffer);
	//RHI::SetConstantBuffer(PIPELINE_STAGE::PIPELINE_STAGE_PIXEL_SHADER, 5, pointLightBuffer);

	ComPtr<ID3D11PixelShader> PointLightPS;
	RHI::CreatePixelShader(PointLightPS,
		BuiltIn_PointLightPS_ByteCode,
		sizeof(BuiltIn_PointLightPS_ByteCode));

	RHI::SetPixelShader(PointLightPS);

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);
	RHI::Draw(4);
}
