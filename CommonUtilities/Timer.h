#pragma once

#include <chrono>

namespace CommonUtilities
{
	class Timer
	{
	public:
		Timer() = default;
		~Timer() = default;

		static void Update();
		static void Reset();

		static float GetDeltaTime();
		static float GetUnscaledDeltaTime();

		static double GetTotalTime();

		static float GetTimeScale();
		static void SetTimeScale(float aValue);

	private:
		static std::chrono::high_resolution_clock::time_point myStartTime;
		static std::chrono::high_resolution_clock::time_point myLastTime;
		static std::chrono::duration<float> myDeltaTime;
		static float myTimeScale;
	};
}
