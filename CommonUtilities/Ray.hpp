#pragma once
#include "Vector3.hpp"

namespace CommonUtilities
{
	template <class T>
	class Ray
	{
	public:
		Ray() = default;

		Ray(const Ray<T>& aRay);

		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);

		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);

		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		const Vector3<T> GetOrigin() const;

		const Vector3<T> GetDirection() const;

	private:
		Vector3<T> origin;
		Vector3<T> direction;
	};

	template <class T>
	Ray<T>::Ray(const Ray<T>& aRay)
	{
		origin = aRay.origin;
		direction = aRay.direction;
	}

	template <class T>
	Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		origin = aOrigin;
		direction = aPoint - aOrigin;
	}

	template <class T>
	void Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		origin = aOrigin;
		direction = aPoint - aOrigin;
	}

	template <class T>
	void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		origin = aOrigin;
		direction = aDirection;
	}

	template <class T>
	const Vector3<T> Ray<T>::GetOrigin() const
	{
		return origin;
	}

	template <class T>
	const Vector3<T> Ray<T>::GetDirection() const
	{
		return direction;
	}
}