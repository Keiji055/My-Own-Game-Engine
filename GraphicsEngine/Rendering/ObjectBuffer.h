#pragma once
#include <CommonUtilities/Matrix4x4.hpp>
#include <CommonUtilities/Vector.hpp>
struct ObjectBufferData
{
	CommonUtilities::Matrix4x4<float>	Transform;
	alignas(4) bool						HasBones;
	unsigned int						ObjectID;
	alignas(4) bool						IsInstanced;
	float								padding;
	CommonUtilities::Matrix4x4<float>	BoneTransform[128];
};
