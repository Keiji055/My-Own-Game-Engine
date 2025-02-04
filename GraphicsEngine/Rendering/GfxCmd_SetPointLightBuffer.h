#pragma once
#include "../GraphicsCommand.h"
#include <GraphicsEngine/Rendering/ConstantBuffer.h>
#include <CommonUtilities/Vector.hpp>
#include <CommonUtilities/Matrix4x4.hpp>

class GfxCmd_SetPointLightBuffer : public GraphicsCommand
{
    CommonUtilities::Vector3f  myColor{};
    CommonUtilities::Vector3f  myPosition{};
    float   myIntensity = 0;
    float   myRange = 0;

public:

    GfxCmd_SetPointLightBuffer(CommonUtilities::Vector3f  aColor, CommonUtilities::Vector3f aPosition, float anIntensity, float aRange);
    
    void Execute() override;
};