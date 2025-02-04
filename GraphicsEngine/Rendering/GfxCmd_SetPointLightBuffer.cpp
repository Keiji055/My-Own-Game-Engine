#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetPointLightBuffer.h"

GfxCmd_SetPointLightBuffer::GfxCmd_SetPointLightBuffer(CommonUtilities::Vector3f aColor, CommonUtilities::Vector3f aPosition, float anIntensity, float aRange)
{
	myColor = aColor;
	myIntensity = anIntensity;
	myPosition = aPosition;
	myRange = aRange;

	GetPointLightBuffer().Data.Color = myColor;
	GetPointLightBuffer().Data.Intensity = myIntensity;
	GetPointLightBuffer().Data.Position = myPosition;
	GetPointLightBuffer().Data.Range = myRange;
}

void GfxCmd_SetPointLightBuffer::Execute()
{
	RHI::UpdateConstantBufferData(GetPointLightBuffer());
	RHI::SetConstantBuffer(PIPELINE_STAGE::PIPELINE_STAGE_PIXEL_SHADER, 5, GetPointLightBuffer());
}
