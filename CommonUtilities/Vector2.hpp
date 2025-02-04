#pragma once
#include <cassert>

namespace CommonUtilities
{
	template <class T>
	class Vector3;

	template <class T>
	class Vector2
	{
	public:
		T x;
		T y;

		Vector2<T>();
		Vector2<T>(const T& aX, const T& aY);
		Vector2<T>(const Vector3<T>& aVector);
		Vector2<T>(const Vector2<T>& aVector) = default;/*
		Vector2<T>(const Tga::Vector2<T>& aVector);*/
		~Vector2<T>() = default;

		//Returns the array vector
		T& operator[](int value);
		const T& operator[](int value) const;

		Vector2<T>& operator=(const Vector2<T>& aVector2) = default;

		template <class U> operator U() const;

		Vector2<T> operator-() const;

		T LengthSqr() const;
		T Length() const;

		Vector2<T> GetNormalized() const;
		void Normalize();

		void Rotate(T aDegrees);

		T Dot(const Vector2<T>& aVector) const;
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int> Vector2i;

	template<class T>
	Vector2<T>::Vector2() :x(0), y(0)
	{
	}

	template <class T>
	Vector2<T>::Vector2(const T& aX, const T& aY) : x(aX), y(aY)
	{
	}

	template <class T>
	Vector2<T>::Vector2(const Vector3<T>& aVector) : x(aVector.x), y(aVector.y)
	{
	}

	//template <class T>
	//Vector2<T>::Vector2(const Tga::Vector2<T>& aVector) : x(aVector.x), y(aVector.y)
	//{
	//}

	template<class T>
	template<class U>
	Vector2<T>::operator U() const
	{
		return { x, y };
	}

	template<class T>
	Vector2<T> Vector2<T>::operator-() const
	{
		return Vector2<T>(-x, -y);
	}

	template <class T>
	T Vector2<T>::LengthSqr() const
	{
		return x * x + y * y;
	}

	template <class T>
	T Vector2<T>::Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	template <class T>
	Vector2<T> Vector2<T>::GetNormalized() const
	{
		T length = Length();
		T lengthInverse = 1 / length;
		return *this * lengthInverse;
	}

	template <class T>
	void Vector2<T>::Normalize()
	{
		T length = Length();
		//assert(length > 0);

		T lengthInverse;
		if (length <= 0)
		{
			lengthInverse = 1;
		}
		else
		{
			lengthInverse = 1 / length;
		}

		x *= lengthInverse;
		y *= lengthInverse;
	}

	template <class T>
	void Vector2<T>::Rotate(T aDegrees)
	{
		double degToRad = 0.0174532925;

		T newX = static_cast<T>(x * cos(static_cast<double>(aDegrees) * degToRad) - y * sin(static_cast<double>(aDegrees) * degToRad));
		T newY = static_cast<T>(y * cos(static_cast<double>(aDegrees) * degToRad) + x * sin(static_cast<double>(aDegrees) * degToRad));

		x = newX;
		y = newY;
	}

	template <class T>
	T Vector2<T>::Dot(const Vector2<T>& anOther) const
	{
		return x * anOther.x + y * anOther.y;
	}


	template <class T>
	Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		Vector2<T> vector(aVector0);
		vector += aVector1;
		return vector;
	}

	template <class T>
	Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		Vector2<T> vector(aVector0);
		vector -= aVector1;
		return vector;
	}

	template <class T>
	Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	template <class T>
	Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		return Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
	}

	template <class T>
	Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0);
		T inverse = 1 / aScalar;

		return aVector * inverse;
	}

	template <class T>
	void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
	}

	template <class T>
	void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
	}

	template <class T>
	void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
	}

	template <class T>
	void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		assert(aScalar != 0);
		T inverse = 1 / aScalar;
		aVector *= inverse;
	}

	template <class T>
	bool operator==(const Vector2<T>& aLeft, const Vector2<T>& aRight)
	{
		return aLeft.x == aRight.x && aLeft.y == aRight.y;
	}


	template<class T>
	inline T& Vector2<T>::operator[](int value)
	{
		assert(value >= 0 && value <= 2);
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
		default:
		{
			return x;
		}
		}
	}

	template<class T>
	inline const T& Vector2<T>::operator[](int value) const
	{
		assert(value >= 0 && value <= 2);
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
		default:
		{
			return x;
		}
		}
	}


}
