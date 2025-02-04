#pragma once
#include <math.h>
#include <random>
#include <cstdlib>
#include <ctime>

namespace CommonUtilities
{
	namespace Mathf
	{
		constexpr float Pi = 3.1415926f;
		constexpr float ToDegMultiplier = 180.0f / Pi;
		constexpr float ToRadMultiplier = Pi / 180.0f;


		template<class T>
		inline T RadToDeg(T aValue) { return static_cast<T>(aValue * ToDegMultiplier); }

		template<class T>
		inline T DegToRad(T aValue) { return static_cast<T>(aValue * ToRadMultiplier); }


		template<class T>
		inline int FloorToInt(T aValue) { return static_cast<int>(std::floor(aValue)); }

		template<class T>
		inline T Floor(T aValue) { return static_cast<T>(std::floor(aValue)); }

		template<class T>
		inline int RoundToInt(T aValue) { return static_cast<int>(std::round(aValue)); }

		template<class T>
		inline T Round(T aValue) { return static_cast<T>(std::round(aValue)); }

		template<class T>
		inline int CeilToInt(T aValue) { return static_cast<int>(std::ceil(aValue)); }

		template<class T>
		inline T Ceil(T aValue) { return static_cast<T>(std::ceil(aValue)); }


		template<class T>
		inline T Min(T aValue, T aValue2) { return (aValue < aValue2) ? aValue : aValue2; }

		template<class T>
		inline T Max(T aValue, T aValue2) { return (aValue < aValue2) ? aValue2 : aValue; }

		template<class T>
		inline T Abs(T aValue) { return (aValue < 0) ? static_cast<T>(aValue * -1) : aValue; }

		template<class T>
		inline T Sign(T aValue) { return (aValue >= 0) ? static_cast<T>(1) : static_cast<T>(-1); }

		template<class T>
		inline T Clamp(T aValue, T aMin, T aMax) { return ((aValue > aMax) ? aMax : (aValue < aMin) ? aMin : aValue); }

		template<class T>
		inline T Clamp01(T aValue) { return Clamp(aValue, T(0), T(1)); }

		template<class T>
		inline T Lerp(T aFrom, T aTo, T aLerpFactor) { return aFrom + (aTo - aFrom) * Clamp01(aLerpFactor); }

		template<class T>
		inline T Random(T aMin, T aMax)
		{
			std::srand(std::time(nullptr));
			T random = std::rand();
			T value = aMin + random / ((RAND_MAX + 1u) / aMax);
			return value;
		}
	}
}