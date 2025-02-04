#include "GraphicsEngine.pch.h"
#include "Animation.h"

Animation::Animation(const TGA::FBX::Animation& aTGAAnim)
{
	for (int index = 0; index < aTGAAnim.Frames.size(); index++)
	{
		Frame tempFrame;
		
		for (auto it = aTGAAnim.Frames[index].LocalTransforms.begin(); it != aTGAAnim.Frames[index].LocalTransforms.end(); ++it)
		{
			CommonUtilities::Matrix4x4<float> tempMatrix;
			tempMatrix(1, 1) = it->second.m11;
			tempMatrix(2, 1) = it->second.m21;
			tempMatrix(3, 1) = it->second.m31;
			tempMatrix(4, 1) = it->second.m41;

			tempMatrix(1, 2) = it->second.m12;
			tempMatrix(2, 2) = it->second.m22;
			tempMatrix(3, 2) = it->second.m32;
			tempMatrix(4, 2) = it->second.m42;

			tempMatrix(1, 3) = it->second.m13;
			tempMatrix(2, 3) = it->second.m23;
			tempMatrix(3, 3) = it->second.m33;
			tempMatrix(4, 3) = it->second.m43;

			tempMatrix(1, 4) = it->second.m14;
			tempMatrix(2, 4) = it->second.m24;
			tempMatrix(3, 4) = it->second.m34;
			tempMatrix(4, 4) = it->second.m44;

			tempFrame.LocalTransforms.emplace(it->first, tempMatrix);
		}
		myFrames.push_back(tempFrame);
	}

	myDuration			= aTGAAnim.Duration;
	myFramesPerSecond	= aTGAAnim.FramesPerSecond;
	myLength			= aTGAAnim.Length;
	myName				= aTGAAnim.Name;
}

const std::vector<Animation::Frame>& Animation::GetFrames() const
{
	return myFrames;
}

const unsigned int& Animation::GetLength() const
{
	return myLength;
}

const float& Animation::GetFPS() const
{
	return myFramesPerSecond;
}
