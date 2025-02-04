#pragma once
#include <wrl.h>
#include <string>
#include <vector>
#include <TGAFbx.h>
#include<CommonUtilities/Matrix4x4.hpp>

#define AsUINT(v)(static_cast<unsigned>(v))
using namespace Microsoft::WRL;

class Animation
{
public:
	struct Frame
	{
		// Stores Joint Name to Transform for Bone Space Transforms.
		std::unordered_map<std::string, CommonUtilities::Matrix4x4<float>> LocalTransforms;
	};

	Animation(const TGA::FBX::Animation& aTGAAnim);
	const std::vector<Frame>& GetFrames() const;
	const unsigned int& GetLength() const;
	const float& GetFPS() const;
private:
	// The animation frames.
	std::vector<Frame> myFrames;

	// How long this animation is in frames.
	unsigned int myLength;

	// The duration of this animation.
	double myDuration;

	// The FPS of this animation.
	float myFramesPerSecond;

	std::string myName;
};

