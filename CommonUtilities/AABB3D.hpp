#pragma once
#include "Vector3.hpp"
#include "Plane.hpp"

namespace CommonUtilities
{
	template <class T>
	class AABB3D
	{
	public:
		AABB3D();

		AABB3D(const AABB3D<T>& aAABB3D);

		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);

		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);

		bool IsInside(const Vector3<T>& aPosition) const;

		const Vector3<T> GetMin() const;

		const Vector3<T> GetMax() const;

	private:
		Vector3<T> myMin;
		Vector3<T> myMax;
	};

	template <class T>
	AABB3D<T>::AABB3D()
	{
		myMin = Vector3<T>();
		myMax = Vector3<T>();
	}

	template <class T>
	AABB3D<T>::AABB3D(const AABB3D<T>& aAABB3D)
	{
		myMin = aAABB3D.myMin; 
		myMax = aAABB3D.myMax;
	}

	template <class T>
	AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMin = aMin;
		myMax = aMax;
	}

	template <class T>
	void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMin = aMin;
		myMax = aMax;
	}

	template <class T>
	bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{
		if (myMin.x <= aPosition.x && aPosition.x <= myMax.x && myMin.y <= aPosition.y && aPosition.y <= myMax.y && myMin.z <= aPosition.z && aPosition.z <= myMax.z) 
		{ 
			return true;
		}         
		return false;
	}

	template <class T>
	const Vector3<T> AABB3D<T>::GetMin() const
	{
		return myMin;
	}

	template <class T>
	const Vector3<T> AABB3D<T>::GetMax() const
	{
		return myMax;
	}
}