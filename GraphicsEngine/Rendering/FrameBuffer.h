#pragma once
#include <CommonUtilities/Matrix4x4.hpp>
#include <CommonUtilities/Vector.hpp>

struct FrameBufferData
{
	CommonUtilities::Matrix4x4<float> View;
	CommonUtilities::Matrix4x4<float> Projection;
	CommonUtilities::Vector3f ViewPosition;
	float padding;
	CommonUtilities::Vector2f Resolution;
	CommonUtilities::Vector2f padding1;
};