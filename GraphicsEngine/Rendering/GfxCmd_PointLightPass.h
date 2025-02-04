#pragma once
#include <GraphicsCommand.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"
#include <CommonUtilities/Vector.hpp>
#include<GraphicsEngine/Rendering/PointLightBuffer.h>
#include <Shaders\Include\PointLightPS.h>

class GfxCmd_PointLightPass : public GraphicsCommand
{
public:
	GfxCmd_PointLightPass(const CommonUtilities::Vector3f& aColor, const float& anIntensity, 
		const CommonUtilities::Vector3f& aPointLightPosition, const float& aRange);
	void Execute();

private:
	CommonUtilities::Vector3f myPointLightColor;
	float myIntensity;
	CommonUtilities::Vector3f myPointLightPosition;
	float myRange;
};