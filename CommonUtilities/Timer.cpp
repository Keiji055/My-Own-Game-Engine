#include "pch.h"
#include "Timer.h"
#include "MathFunctions.hpp"

namespace CommonUtilities
{
	void Timer::Update()
	{
		myDeltaTime = std::chrono::high_resolution_clock::now() - myLastTime;
		myLastTime = std::chrono::high_resolution_clock::now();
	}

	void Timer::Reset()
	{
		myTimeScale = 1;
		myStartTime = std::chrono::high_resolution_clock::now();
		myLastTime = std::chrono::high_resolution_clock::now();
	}

	float Timer::GetDeltaTime()
	{
		return myDeltaTime.count() * myTimeScale;
	}

	float Timer::GetUnscaledDeltaTime()
	{
		return myDeltaTime.count();
	}

	double Timer::GetTotalTime()
	{
		return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - myStartTime).count();
	}

	float Timer::GetTimeScale()
	{
		return myTimeScale;
	}

	void Timer::SetTimeScale(float aValue)
	{
		myTimeScale = Mathf::Clamp(aValue, 0.0f, static_cast<float>(INT32_MAX));
	}

	std::chrono::high_resolution_clock::time_point Timer::myStartTime;
	std::chrono::high_resolution_clock::time_point Timer::myLastTime;
	std::chrono::duration<float> Timer::myDeltaTime;
	float Timer::myTimeScale;
}
