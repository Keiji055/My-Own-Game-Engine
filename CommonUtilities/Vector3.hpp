#pragma once
#include <cassert>
#include <ostream>

namespace CommonUtilities
{
	template <class T>
	class Vector4;

	template <class T>
	class Vector2;

	template <class T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;

		Vector3<T>();
		Vector3<T>(const T& aX, const T& aY, const T& aZ);
		Vector3<T>(const Vector3<T>& aVector) = default;
		Vector3<T>(const Vector4<T>& aVector4);/*
		Vector3<T>(const Tga::Vector3<T> aVector);*/
		Vector3<T>(const Vector2<T> aVector);
		~Vector3<T>() = default;

		//Returns the array vector
		T& operator[](int value);
		const T& operator[](int value) const;


		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;

		template <class U> operator U() const;

		Vector3<T> operator-() const;
		T LengthSqr() const;
		T Length() const;

		Vector3<T> GetNormalized() const;
		void Normalize();

		T Dot(const Vector3<T>& aVector) const;
		Vector3<T> Cross(const Vector3<T>& aVector) const;

		static Vector3<T> Up;
		static Vector3<T> Right;
		static Vector3<T> Forward;
		static Vector3<T> Down;
		static Vector3<T> Left;
		static Vector3<T> Back;
		static Vector3<T> Zero;

	};

	typedef Vector3<float> Vector3f;
	typedef Vector3<int> Vector3i;

	template<class T>
	Vector3<T>::Vector3() :x(0), y(0), z(0)
	{
	}

	template <class T>
	Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ) : x(aX), y(aY), z(aZ)
	{
	}

	template <class T>
	Vector3<T>::Vector3(const Vector4<T>& aVector4) : x(aVector4.x), y(aVector4.y), z(aVector4.z)
	{
	}

	//template <class T>
	//Vector3<T>::Vector3(const Tga::Vector3<T> aVector):x(aVector.x), y(aVector.y), z(aVector.z)
	//{
	//}

	template <class T>
	Vector3<T>::Vector3(const Vector2<T> aVector) : x(aVector.x), y(aVector.y), z(0)
	{
	}


	template<class T>
	template<class U>
	Vector3<T>::operator U() const
	{
		return { x, y, z };
	}

	template<class T>
	Vector3<T> Vector3<T>::operator-() const
	{
		return Vector3<T>(-x, -y, -z);
	}

	template <class T>
	T Vector3<T>::LengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	template <class T>
	T Vector3<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template <class T>
	Vector3<T> Vector3<T>::GetNormalized() const
	{
		T length = Length();
		assert(length > 0);
		T lengthInverse = 1 / length;
		return *this * lengthInverse;
	}

	template <class T>
	void Vector3<T>::Normalize()
	{
		T length = Length();
		assert(length > 0);

		T lengthInverse = 1 / length;
		x *= lengthInverse;
		y *= lengthInverse;
		z *= lengthInverse;
	}

	template <class T>
	T Vector3<T>::Dot(const Vector3<T>& anOther) const
	{
		return x * anOther.x + y * anOther.y + z * anOther.z;
	}

	template <class T>
	Vector3<T> Vector3<T>::Cross(const Vector3<T>& anOther) const
	{
		return Vector3<T>
			(
				y * anOther.z - z * anOther.y,
				z * anOther.x - x * anOther.z,
				x * anOther.y - y * anOther.x
			);
	}

	template <class T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		Vector3<T> vector(aVector0);
		vector += aVector1;
		return vector;
	}

	template <class T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		Vector3<T> vector(aVector0);
		vector -= aVector1;
		return vector;
	}

	template <class T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}

	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		return Vector3<T>(aVector0.x * aVector1.x, aVector0.y * aVector1.y, aVector0.z * aVector1.z);
	}

	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
	}

	template <class T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0);
		T inverse = 1 / aScalar;

		return aVector * inverse;
	}

	template <class T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
	}

	template <class T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
	}

	template <class T>
	void operator*=(Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		lhs = lhs * rhs;
	}

	template <class T>
	void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0);
		T inverse = 1 / aScalar;
		aVector *= inverse;
	}

	template<class T>
	inline T& Vector3<T>::operator[](int value)
	{
		assert(value >= 0 && value <= 3);
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
		default:
		{
			return x;
		}
		}
	}





	template<class T>
	inline const T& Vector3<T>::operator[](int value) const
	{
		assert(value >= 0 && value <= 3);
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
		default:
		{
			return x;
		}
		}
	}

	template <class T>
	std::ostream& operator<<(std::ostream& anOstream, const Vector3<T>& aVector)
	{
		anOstream << "{x: " << aVector.x << ", y: " << aVector.y << ", z: " << aVector.z << "}";
		return anOstream;
	}

	template <class T>
	Vector3<T> Vector3<T>::Up(0, 1, 0);
	template <class T>
	Vector3<T> Vector3<T>::Down(0, -1, 0);
	template <class T>
	Vector3<T> Vector3<T>::Right(1, 0, 0);
	template <class T>
	Vector3<T> Vector3<T>::Left(-1, 0, 0);
	template <class T>
	Vector3<T> Vector3<T>::Forward(0, 0, 1);
	template <class T>
	Vector3<T> Vector3<T>::Back(0, 0, -1);

	template <class T>
	Vector3<T> Vector3<T>::Zero(0, 0, 0);

}