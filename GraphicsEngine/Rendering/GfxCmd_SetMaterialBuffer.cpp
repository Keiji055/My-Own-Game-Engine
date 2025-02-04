#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetMaterialBuffer.h"

GfxCmd_SetMaterialBuffer::GfxCmd_SetMaterialBuffer(const CommonUtilities::Vector4f& anAlbedoColor, const CommonUtilities::Vector2f& aUVTiling, float aNormalStrength, float aShininess)
{
	myAlbedoColor = anAlbedoColor;
	myUVTiling = aUVTiling;
	myNormalStrength = aNormalStrength;
	myShininess = aShininess;
	GetMaterialBuffer().Data.AlbedoColor = myAlbedoColor;
	GetMaterialBuffer().Data.UVTiling = myUVTiling;
	GetMaterialBuffer().Data.NormalStrength = myNormalStrength;
	GetMaterialBuffer().Data.Shininess = myShininess;
}

void GfxCmd_SetMaterialBuffer::Execute()
{
	MaterialBuffer& materialBuffer = GetMaterialBuffer();
	RHI::UpdateConstantBufferData(materialBuffer);
	RHI::SetConstantBuffer(PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 2, materialBuffer);
}
