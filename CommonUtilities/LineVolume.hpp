#pragma once
#include "Vector2.hpp"
#include "Line.hpp"
#include <vector>

namespace CommonUtilities
{
	template <class T>
	class LineVolume
	{
	public:
		LineVolume();

		LineVolume(const std::vector<Line<T>>& aLineList);

		void AddLine(const Line<T>& aLine);

		bool IsInside(const Vector2<T>& aPosition);

	private:
		std::vector<Line> polygon;
	};

	template <class T>
	LineVolume<T>::LineVolume()
	{
		polygon = std::vector<Line<T>>();
	}

	template <class T>
	LineVolume<T>::LineVolume(const std::vector<Line<T>>& aLineList)
	{
		polygon = aLineList;
	}

	template <class T>
	void LineVolume<T>::AddLine(const Line<T>& aLine)
	{
		polygon.push_back(aLine);
	}

	template <class T>
	bool LineVolume<T>::IsInside(const Vector2<T>& aPosition)
	{
		for (int index = 0; index < polygon.size(); index++)
		{
			if (polygon[index].IsInside(aPosition) == false)
			{
				return false;
			}
		}
		return true;
	}
}