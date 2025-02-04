#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetLightBuffer.h"
#include <GraphicsEngine/InterOp/RHI.h>
#include <GraphicsEngine.h>
#include "LightBuffer.h"

GfxCmd_SetLightBuffer::GfxCmd_SetLightBuffer(Matrix4x4<float> aViewProj, Matrix4x4<float> aLightProj, Vector4f aColor, Vector3f aDirection, float anIntensity)
{
	myColor = aColor;
	myDirection = aDirection;
	myIntensity = anIntensity;
	myViewProj = aViewProj;
	myLightProj = aLightProj;

	GetLightBuffer().Data.Color = myColor;
	GetLightBuffer().Data.Direction = myDirection;
	GetLightBuffer().Data.Intensity = myIntensity;
	GetLightBuffer().Data.Position = myPosition;
	GetLightBuffer().Data.ViewMatrix = myViewProj.GetFastInverse(myViewProj);
	GetLightBuffer().Data.ProjectionMatrix = myLightProj;
}

GfxCmd_SetLightBuffer::GfxCmd_SetLightBuffer(Vector4f aColor, Vector3f aPosition, float anIntensity, float aRange)
{
	myColor = aColor;
	myPosition = aPosition;
	myRange = aRange;
	myIntensity = anIntensity;

	GetPointLightBuffer().Data.Color = myColor;
	GetPointLightBuffer().Data.Position = myPosition;
	GetPointLightBuffer().Data.Range = myRange;
	GetPointLightBuffer().Data.Intensity = myIntensity;
}

GfxCmd_SetLightBuffer::GfxCmd_SetLightBuffer(Vector4f aColor, Vector3f aPosition, Vector3f aDirection, float anIntensity, float aRange, float aConeRadius, float aPenumbra, int aLightID)
{
	myColor = aColor;
	myPosition = aPosition;
	myDirection = aDirection;
	myRange = aRange;
	myIntensity = anIntensity;
	myConeRadius = aConeRadius;
	myPenumbra = aPenumbra;

	GetSpotLightBuffer().Data.Color = myColor;
	GetSpotLightBuffer().Data.Position = myPosition;
	GetSpotLightBuffer().Data.Direction = myDirection;
	GetSpotLightBuffer().Data.Range = myRange;
	GetSpotLightBuffer().Data.Intensity = myIntensity;
	GetSpotLightBuffer().Data.ConeRadius = myConeRadius;
	GetSpotLightBuffer().Data.Penumbra = myPenumbra;
}

void GfxCmd_SetLightBuffer::Execute()
{
	RHI::UpdateConstantBufferData(GetLightBuffer());
	RHI::SetConstantBuffer(PIPELINE_STAGE::PIPELINE_STAGE_PIXEL_SHADER, 3, GetLightBuffer());
}

