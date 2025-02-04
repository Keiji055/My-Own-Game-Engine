#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class Sphere
	{
	public:
		Sphere();

		Sphere(const Sphere<T>& aSphere);

		Sphere(const Vector3<T>& aCenter, T aRadius);

		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);

		bool IsInside(const Vector3<T>& aPosition) const;

		const Vector3<T> GetCenter() const;

		const T GetRadius() const;

	private:
		Vector3<T> center;
		T radius;
	};

	template <class T>
	Sphere<T>::Sphere()
	{
		center = Vector3<T>(0, 0, 0);
		radius = T(0);
	}

	template <class T>
	Sphere<T>::Sphere(const Sphere<T>& aSphere)
	{
		center = aSphere.center;
		radius = aSphere.radius;
	}

	template <class T>
	Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius)
	{
		center = aCenter;
		radius = aRadius;
	}

	template <class T>
	void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		center = aCenter;
		radius = aRadius;
	}

	template <class T>
	bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		if ((aPosition - center).Length() <= radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template <class T>
	const Vector3<T> Sphere<T>::GetCenter() const
	{
		return center;
	}

	template <class T>
	const T Sphere<T>::GetRadius() const
	{
		return radius;
	}
}