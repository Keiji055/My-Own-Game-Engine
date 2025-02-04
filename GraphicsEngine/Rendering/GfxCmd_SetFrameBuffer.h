#pragma once
#include <GraphicsCommand.h>
#include <CommonUtilities/Matrix4x4.hpp>
#include <GraphicsEngine/Rendering/ConstantBuffer.h>
#include <GraphicsEngine/Rendering/Vertex.h>
#include <memory>
class GfxCmd_SetFrameBuffer : public GraphicsCommand
{
	CommonUtilities::Matrix4x4<float> myView;
	CommonUtilities::Matrix4x4<float> myProjection;
	CommonUtilities::Vector3f myViewPosition;
	CommonUtilities::Vector2f myResolution;
public:

	GfxCmd_SetFrameBuffer(const CommonUtilities::Matrix4x4<float>& aViewMatix,
						  const CommonUtilities::Matrix4x4<float>& aProjectionMatrix,
						  const CommonUtilities::Vector3f& aViewPostition, 
					      const CommonUtilities::Vector2f& aResolution);

	void Execute() override;
};
