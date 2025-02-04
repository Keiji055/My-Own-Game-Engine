#include "GraphicsEngine.pch.h"
#include "GfxCmd_SpotLightPass.h"

GfxCmd_SpotLightPass::GfxCmd_SpotLightPass(CommonUtilities::Vector3f aColor, CommonUtilities::Vector3f aPosition, 
	CommonUtilities::Vector3f aDirection, float anIntensity, float aRange, float aConeRadius, float aPenumbra)
{
	myColor = aColor;
	myDirection = aDirection;
	myPosition = aPosition;
	myRange = aRange;
	myIntensity = anIntensity;
	myConeRadius = aConeRadius;
	myPenumbra = aPenumbra;
}

void GfxCmd_SpotLightPass::Execute()
{
	SpotLightBuffer& spotLightBuffer = GetSpotLightBuffer();
	spotLightBuffer.Data.Color = myColor;
	spotLightBuffer.Data.Direction = myDirection;
	spotLightBuffer.Data.Intensity = myIntensity;
	spotLightBuffer.Data.Position = myPosition;
	spotLightBuffer.Data.Range = myRange;
	spotLightBuffer.Data.ConeRadius = myConeRadius;
	spotLightBuffer.Data.Penumbra = myPenumbra;

	RHI::UpdateConstantBufferData(spotLightBuffer);
	//RHI::SetConstantBuffer(PIPELINE_STAGE::PIPELINE_STAGE_PIXEL_SHADER, 5, pointLightBuffer);

	ComPtr<ID3D11PixelShader> spotLightPS;
	RHI::CreatePixelShader(spotLightPS,
		BuiltIn_SpotLightPS_ByteCode,
		sizeof(BuiltIn_SpotLightPS_ByteCode));

	RHI::SetPixelShader(spotLightPS);

	RHI::ConfigureInputAssembler(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		nullptr,
		nullptr,
		0,
		nullptr
	);
	RHI::Draw(4);
}