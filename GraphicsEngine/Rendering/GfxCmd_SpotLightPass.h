#pragma once
#include <GraphicsCommand.h>
#include "../GraphicsEngine.h"
#include "../InterOp/RHI.h"
#include <Shaders\Include\SpotLightPS.h>

class GfxCmd_SpotLightPass : public GraphicsCommand
{
public:
	GfxCmd_SpotLightPass(CommonUtilities::Vector3f aColor, CommonUtilities::Vector3f aPosition, 
        CommonUtilities::Vector3f aDirection, float anIntensity, float aRange, float aConeRadius, float aPenumbra);
	void Execute();

private:
    CommonUtilities::Vector3f  myColor{};
    CommonUtilities::Vector3f  myDirection{};
    CommonUtilities::Vector3f  myPosition{};
    CommonUtilities::Matrix4x4<float> myViewProj;
    CommonUtilities::Matrix4x4<float> myLightProj;

    float   myIntensity = 0;
    float   myRange = 0;
    float   myConeRadius = 0;
    float   myPenumbra = 0;
};