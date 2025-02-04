#pragma once
#include <CommonUtilities/Vector.hpp>

struct PointLightData
{
    CommonUtilities::Vector3f  Color;
    float   Intensity;
    CommonUtilities::Vector3f  Position;
    float   Range;  
};