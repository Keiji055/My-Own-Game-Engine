#include "GraphicsEngine.pch.h"
#include "GfxCmd_SetFrameBuffer.h"
#include <GraphicsEngine/InterOp/RHI.h>
#include <GraphicsEngine.h>
#include <GraphicsEngine/Rendering/FrameBuffer.h>
GfxCmd_SetFrameBuffer::GfxCmd_SetFrameBuffer(const CommonUtilities::Matrix4x4<float>& aViewMatix, const CommonUtilities::Matrix4x4<float>& aProjectionMatrix, 
	const CommonUtilities::Vector3f& aViewPostition, const CommonUtilities::Vector2f& aResolution)
{
	myView = CommonUtilities::Matrix4x4<float>::GetFastInverse(aViewMatix);
	myProjection = aProjectionMatrix;
	myViewPosition = aViewPostition;
	myResolution = aResolution;
}

void GfxCmd_SetFrameBuffer::Execute()
{
	GetFrameBuffer().Data.Projection = myProjection;
	GetFrameBuffer().Data.View = myView;
	GetFrameBuffer().Data.ViewPosition = myViewPosition;
	GetFrameBuffer().Data.Resolution = myResolution;
	RHI::UpdateConstantBufferData(GetFrameBuffer());
	RHI::SetConstantBuffer(PIPELINE_STAGE::PIPELINE_STAGE_VERTEX_SHADER | PIPELINE_STAGE_GEOMETERY_SHADER | PIPELINE_STAGE_PIXEL_SHADER, 0, GetFrameBuffer());
}
