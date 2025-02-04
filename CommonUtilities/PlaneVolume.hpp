#pragma once
#include "Vector3.hpp"
#include "Plane.hpp"
#include <vector>

namespace CommonUtilities
{
	template <class T>
	class PlaneVolume
	{
	public:
		PlaneVolume();

		PlaneVolume(const std::vector<Plane<T>>& aPlaneList);

		void AddPlane(const Plane<T>& aPlane);

		bool IsInside(const Vector3<T>& aPosition) const;
		
	private:
		std::vector<Plane<T>> polyhedron;
	};

	template <class T>
	PlaneVolume<T>::PlaneVolume()
	{
		polyhedron = std::vector<Plane<T>>();
	}

	template <class T>
	PlaneVolume<T>::PlaneVolume(const std::vector<Plane<T>>& aPlaneList)
	{
		polyhedron = aPlaneList;
	}

	template <class T>
	void PlaneVolume<T>::AddPlane(const Plane<T>& aPlane)
	{
		polyhedron.push_back(aPlane);
	}

	template <class T>
	bool PlaneVolume<T>::IsInside(const Vector3<T>& aPosition) const
	{
		for (int index = 0; index < polyhedron.size(); index++)
		{
			if (polyhedron[index].IsInside(aPosition) == false)
			{
				return false;
			}
		}
		return true;
	}
}