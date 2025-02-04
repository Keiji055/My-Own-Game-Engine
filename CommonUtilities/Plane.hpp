#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class Plane
	{
	public:
		Plane();

		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		bool IsInside(const Vector3<T>& aPosition) const;

		const Vector3<T>& GetNormal() const;

		const Vector3<T> GetPoint() const;

	private:
		Vector3<T> normal;
		Vector3<T> point;
	};

	template <class T>
	Plane<T>::Plane()
	{
		normal = 0;
		point = 0;
	}

	template <class T>
	Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		Vector3<T> d1 = aPoint1 - aPoint0;
		Vector3<T> d2 = aPoint2 - aPoint0;
		point = aPoint0;
		normal = d1.Cross(d2);
 	}

	template <class T>
	Plane<T>::Plane(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		point = aPoint;
		normal = aNormal;
	}

	template <class T>
	void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		Vector3<T> d1 = aPoint1 - aPoint0;
		Vector3<T> d2 = aPoint2 - aPoint0;
		point = aPoint0;
		normal = d1.Cross(d2);
	}

	template <class T>
	void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		point = aPoint;
		normal = aNormal;
	}

	template <class T>
	bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		Vector3<T> pq = point - aPosition;
		if (pq.Dot(normal) >= 0)
			return true;
		else
			return false;
	}

	template <class T>
	const Vector3<T>& Plane<T>::GetNormal() const
	{
		return normal;
	}

	template<class T>
	const Vector3<T> Plane<T>::GetPoint() const
	{
		return point;
	}
}