#pragma once
#include "../GraphicsCommand.h"
#include <GraphicsEngine/Rendering/ConstantBuffer.h>
#include <CommonUtilities/Vector.hpp>
#include <CommonUtilities/Matrix4x4.hpp>

class GfxCmd_SetLightBuffer : public GraphicsCommand
{
    Vector4f  myColor{};
    Vector3f  myDirection{};
    Vector3f  myPosition{};
    Matrix4x4<float> myViewProj;
    Matrix4x4<float> myLightProj;

    float   myIntensity = 0;
    float   myRange = 0;
    float   myConeRadius = 0;
    float   myPenumbra = 0;
    unsigned     myPointLightCounter = 0;
    unsigned int    mySpotLightCounter = 0;

public:

    GfxCmd_SetLightBuffer(Matrix4x4<float> aViewProj, Matrix4x4<float> aLightProj, Vector4f  aColor, Vector3f  aDirection, float anIntensity);
    GfxCmd_SetLightBuffer(Vector4f  aColor, Vector3f aPosition, float anIntensity ,float aRange);
    GfxCmd_SetLightBuffer(Vector4f  aColor, Vector3f aPosition, Vector3f aDirection, float anIntensity, float aRange,  float aConeRadius, float aPenumbra, int aLightID);

    void Execute() override;
};