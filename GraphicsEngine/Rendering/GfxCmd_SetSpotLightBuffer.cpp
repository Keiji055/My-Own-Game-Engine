#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetSpotLightBuffer.h"

GfxCmd_SetSpotLightBuffer::GfxCmd_SetSpotLightBuffer(CommonUtilities::Vector3f aColor, CommonUtilities::Vector3f aPosition, 
	CommonUtilities::Vector3f aDirection, float anIntensity, float aRange, float aConeRadius, float aPenumbra)
{
	myColor = aColor;
	myDirection = aDirection;
	myPosition = aPosition;
	myRange = aRange;
	myIntensity = anIntensity;
	myConeRadius = aConeRadius;
	myPenumbra = aPenumbra;

	GetSpotLightBuffer().Data.Color = myColor;
	GetSpotLightBuffer().Data.Direction = myDirection;
	GetSpotLightBuffer().Data.Position = myPosition;
	GetSpotLightBuffer().Data.Range = myRange;
	GetSpotLightBuffer().Data.Intensity = myIntensity;
	GetSpotLightBuffer().Data.ConeRadius = myConeRadius;
	GetSpotLightBuffer().Data.Penumbra = myPenumbra;
}

void GfxCmd_SetSpotLightBuffer::Execute()
{
	RHI::UpdateConstantBufferData(GetSpotLightBuffer());
	RHI::SetConstantBuffer(PIPELINE_STAGE::PIPELINE_STAGE_PIXEL_SHADER, 6, GetSpotLightBuffer());
}
