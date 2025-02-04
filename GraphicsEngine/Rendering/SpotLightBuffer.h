#pragma once
#include <CommonUtilities/Vector.hpp>

struct SpotLightData
{
    CommonUtilities::Vector3f  Color;
    float   Intensity;
    CommonUtilities::Vector3f  Position;
    float   Range;
    CommonUtilities::Vector3f  Direction;
    float   ConeRadius;
    float   Penumbra;
    CommonUtilities::Vector3f  padding;
};