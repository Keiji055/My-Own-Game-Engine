#pragma once
#include "AABB3D.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vector3.hpp"
#include "Plane.hpp"

namespace CommonUtilities
{
	template <class T>
	inline bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		T d = aPlane.GetPoint().Dot(aPlane.GetNormal());
		T D = aRay.GetDirection().GetNormalized().Dot(aPlane.GetNormal());

		if (D == T(0))
		{
			if (aPlane.GetNormal().Dot(aPlane.GetPoint() - aRay.GetOrigin()) == 0)
			{
				return true;
			}
			return false;
		}

		T t = (d - aRay.GetOrigin().Dot(aPlane.GetNormal())) / D;

		Vector3<T> collisionPoint = aRay.GetOrigin() + (aRay.GetDirection().GetNormalized() * t);

		if (t >= T(0))
		{
			aOutIntersectionPoint = collisionPoint;
			return true;
		}
		else
		{
			return false;
		}
	}

	template <class T>
	inline bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay)
	{
		Vector3<T> rayDelta;
		T xt;
		T yt;
		T zt;
		
		if (aAABB.IsInside(aRay.GetOrigin()))
		{
			return true;
		}

		//Calculate rayDelta
		if (aAABB.GetMax().x - aRay.GetOrigin().x > aAABB.GetMax().y - aRay.GetOrigin().y && aAABB.GetMax().x - aRay.GetOrigin().x > aAABB.GetMax().z - aRay.GetOrigin().z)
		{
			rayDelta = aRay.GetOrigin() + (aRay.GetDirection().GetNormalized() * (aAABB.GetMax().x - aRay.GetOrigin().x));
		}
		else if(aAABB.GetMax().y - aRay.GetOrigin().y > aAABB.GetMax().x - aRay.GetOrigin().x && aAABB.GetMax().y - aRay.GetOrigin().y > aAABB.GetMax().z - aRay.GetOrigin().z)
		{
			rayDelta = aRay.GetOrigin() + (aRay.GetDirection().GetNormalized() * (aAABB.GetMax().y - aRay.GetOrigin().y));
		}
		else if (aAABB.GetMax().z - aRay.GetOrigin().z > aAABB.GetMax().x - aRay.GetOrigin().x && aAABB.GetMax().z - aRay.GetOrigin().z > aAABB.GetMax().y - aRay.GetOrigin().y)
		{
			rayDelta = aRay.GetOrigin() + (aRay.GetDirection().GetNormalized() * (aAABB.GetMax().z - aRay.GetOrigin().z));
		}



		if (aRay.GetOrigin().x < aAABB.GetMin().x)
		{
			xt = aAABB.GetMin().x - aRay.GetOrigin().x;

			if (xt > rayDelta.x)
			{
				return false;
			}
			//Divide xt with rayDelta to shorten the vector so that it is within the AABB
			xt /= rayDelta.x;
		}
		else if (aRay.GetOrigin().x > aAABB.GetMax().x)
		{
			xt = aAABB.GetMax().x - aRay.GetOrigin().x;

			if (xt < rayDelta.x)
			{
				return false;
			}
			xt /= rayDelta.x;
		}

		if (aRay.GetOrigin().y < aAABB.GetMin().y)
		{
			yt = aAABB.GetMin().y - aRay.GetOrigin().y;
			if (yt > rayDelta.y)
			{
				return false;
			}
			yt /= rayDelta.y;
		}
		else if (aRay.GetOrigin().y > aAABB.GetMax().y)
		{
			yt = aAABB.GetMax().y - aRay.GetOrigin().y;
			if (yt < rayDelta.y)
			{
				return false;
			}
			yt /= rayDelta.y;
		}

		if (aRay.GetOrigin().z < aAABB.GetMin().z)
		{
			zt = aAABB.GetMin().z - aRay.GetOrigin().z;
			if (zt > rayDelta.z)
			{
				return false;
			}
			zt /= rayDelta.z;
		}
		else if (aRay.GetOrigin().z > aAABB.GetMax().z)
		{
			zt = aAABB.GetMax().z - aRay.GetOrigin().z;
			if (zt < rayDelta.z)
			{
				return false;
			}
			zt /= rayDelta.z;
		}

		return true;
	}

	template <class T>
	inline bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		Vector3<T> centerToRayOrigin = aSphere.GetCenter() - aRay.GetOrigin();
		
		T vProj = centerToRayOrigin.Dot(aRay.GetDirection().GetNormalized());
		
		Vector3<T> Pn = aRay.GetOrigin() + (aRay.GetDirection().GetNormalized() * vProj);

		if (aSphere.IsInside(Pn))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}