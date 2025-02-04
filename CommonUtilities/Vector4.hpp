#pragma once
#include <math.h>
#include <cassert>
#include <iostream>

namespace CommonUtilities
{
	template <class T>
	class Vector3;

	template <class T>
	class Vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		Vector4<T>();
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);
		Vector4<T>(const Vector4<T>& aVector) = default;
		Vector4<T>(const Vector3<T>& aVector);
		Vector4<T>(const Vector3<T>& aVector, const T& aW);
		~Vector4<T>() = default;

		//Returns the array vector
		T& operator[](int value);
		const T& operator[](int value) const;

		Vector4<T>& operator=(const Vector4<T>& aVector4) = default;

		template <class U> operator U() const;

		Vector4<T> operator-() const;
		T LengthSqr() const;
		T Length() const;

		Vector4<T> GetNormalized() const;
		void Normalize();

		T Dot(const Vector4<T>& aVector) const;

		static Vector4<T> Forward;
		static Vector4<T> Up;
		static Vector4<T> Right;
	};

	typedef Vector4<float> Vector4f;
	typedef Vector4<int> Vector4i;

	template<class T>
	Vector4<T>::Vector4() :x(0), y(0), z(0), w(0)
	{
	}

	template <class T>
	Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW) : x(aX), y(aY), z(aZ), w(aW)
	{
	}

	template <class T>
	Vector4<T>::Vector4(const Vector3<T>& aVector) : x(aVector.x), y(aVector.y), z(aVector.z), w(0)
	{
	}


	template <class T>
	Vector4<T>::Vector4(const Vector3<T>& aVector, const T& aW) : x(aVector.x), y(aVector.y), z(aVector.z), w(aW)
	{
	}

	template<class T>
	template<class U>
	Vector4<T>::operator U() const
	{
		return { x, y, z, w };
	}

	template<class T>
	Vector4<T> Vector4<T>::operator-() const
	{
		return Vector4<T>(-x, -y, -z, -w);
	}

	template <class T>
	T Vector4<T>::LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	template <class T>
	T Vector4<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template <class T>
	Vector4<T> Vector4<T>::GetNormalized() const
	{
		T length = Length();
		assert(length > 0);
		T lengthInverse = 1 / length;
		return *this * lengthInverse;
	}

	template <class T>
	void Vector4<T>::Normalize()
	{
		T length = Length();
		assert(length > 0);

		T lengthInverse = 1 / length;
		x *= lengthInverse;
		y *= lengthInverse;
		z *= lengthInverse;
		w *= lengthInverse;
	}

	template <class T>
	T Vector4<T>::Dot(const Vector4<T>& anOther) const
	{
		return x * anOther.x + y * anOther.y + z * anOther.z + w * anOther.w;
	}

	template <class T>
	Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		Vector4<T> vector(aVector0);
		vector += aVector1;
		return vector;
	}

	template <class T>
	Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		Vector4<T> vector(aVector0);
		vector -= aVector1;
		return vector;
	}

	template <class T>
	Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	template <class T>
	Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0);
		T inverse = 1 / aScalar;

		return aVector * inverse;
	}

	template <class T>
	void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		aVector0.w += aVector1.w;
	}

	template <class T>
	void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		aVector0.w -= aVector1.w;
	}

	template <class T>
	void operator*=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
		aVector.w *= aScalar;
	}

	template <class T>
	void operator/=(Vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0);
		T inverse = 1 / aScalar;
		aVector *= inverse;
	}

	template <class T>
	std::ostream& operator<<(std::ostream& anOstream, const Vector4<T>& aVector)
	{
		anOstream << "{x: " << aVector.x << ", y: " << aVector.y << ", z: " << aVector.z << ", w: " << aVector.w << "}";
		return anOstream;
	}

	template<class T>
	inline T& Vector4<T>::operator[](int value)
	{
		assert(value >= 0 && value <= 4);
		switch (value)
		{
		case 0:
		{
			return x;
		}
		case 1:
		{
			return y;
		}
		case 2:
		{
			return z;
		}
		case 3:
		{
			return w;
		}
		default:
		{
			return x;
		}
		}
	}

	template<class T>
	inline const T& Vector4<T>::operator[](int value) const
	{
		assert(value >= 0 && value <= 4);
		switch (value)
		{
		case 0:
		{
			return x;
		}
		case 1:
		{
			return y;
		}
		case 2:
		{
			return z;
		}
		case 3:
		{
			return w;
		}
		default:
		{
			return x;
		}
		}
	}


	template<class T>
	Vector4<T> Vector4<T>::Forward(0, 0, 1, 0);
	template<class T>
	Vector4<T> Vector4<T>::Up(0, 1, 0, 0);
	template<class T>
	Vector4<T> Vector4<T>::Right(1, 0, 0, 0);
}

namespace CU = CommonUtilities;