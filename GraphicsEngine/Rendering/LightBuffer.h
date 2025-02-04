#pragma once
#include <CommonUtilities/Vector.hpp>

struct DirectionalLightData
{
    CommonUtilities::Matrix4x4<float> ProjectionMatrix;
    CommonUtilities::Matrix4x4<float> ViewMatrix;
    CommonUtilities::Vector3f Position;
    float   padding0;
    CommonUtilities::Vector3f  Color;
    float   Intensity;
    CommonUtilities::Vector3f  Direction;
    float   padding1;
};
