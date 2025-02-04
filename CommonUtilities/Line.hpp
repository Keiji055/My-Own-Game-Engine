#pragma once
#include "Vector2.hpp"

namespace CommonUtilities
{
	template <class T>
	class Line
	{
	public:
		Line();
		
		Line(const Line <T>& aLine);

		Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1);
		
		void InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T> aPoint1);

		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);

		bool IsInside(const Vector2<T>& aPosition) const;

		const Vector2<T>& GetDirection() const;
		const Vector2<T> GetNormal() const;

	private:
		Vector2<T> point;
		Vector2<T> direction;
	};

	template <class T>
	Line<T>::Line()
	{
		point = (0, 0);
		direction = (0, 0);
	}

	template <class T>
	Line<T>::Line(const Line<T>& aLine)
	{
		point = aLine.point;
		direction = aLine.direction;
	}

	template <class T>
	Line<T>::Line(const Vector2<T>& aPoint0, const Vector2<T>& aPoint1)
	{
		Vector2<T> temp = aPoint1 - aPoint0;
		point = aPoint0;
		direction = temp.GetNormalized();
	}

	template <class T>
	void Line<T>::InitWith2Points(const Vector2<T>& aPoint0, const Vector2<T> aPoint1)
	{
		Vector2<T> temp = aPoint1 - aPoint0;
		point = aPoint0;
		direction = temp.GetNormalized();
	}

	template <class T>
	void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		point = aPoint;
		direction = aDirection;
	}

	template <class T>
	bool Line<T>::IsInside(const Vector2<T>& aPoint) const
	{
		Vector2<T> normal = (-direction.y, direction.x);
		if (direction.Dot(normal) <= 0)
		{
			return true;
		}
		else
			return false;
	}

	template <class T>
	const Vector2<T>& Line<T>::GetDirection() const
	{
		return direction;
	}

	template <class T>
	const Vector2<T> Line<T>::GetNormal() const
	{
		return Vector2<T>(-direction.y, direction.x);
	}
}