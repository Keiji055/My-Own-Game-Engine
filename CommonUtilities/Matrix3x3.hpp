#pragma once
#include <array>
#include "Matrix4x4.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<class T>
	class Matrix3x3
	{
	public:
		Matrix3x3<T>();
		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);
		Matrix3x3<T>(std::initializer_list<T> aList);
		~Matrix3x3() = default;

		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		T& operator[](const int aIndex);
		const T& operator[](const int aIndex) const;

		Matrix3x3<T>& operator=(const Matrix3x3<T>& aMetrix);

		static Matrix3x3<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(const T aAngleInRadians);

		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

	private:
		std::array<T, 9> myArray;
	};

	template <class T>
	Matrix3x3<T> operator+(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> tempMatrix;

		for (int index = 0; index < 9; index++)
		{
			tempMatrix[index] = aMatrix0[index] + aMatrix1[index];
		}

		return tempMatrix;
	}

	template <class T>
	Matrix3x3<T> operator+=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		return aMatrix0 = aMatrix0 + aMatrix1;
	}

	template <class T>
	Matrix3x3<T> operator-(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> tempMatrix;

		for (int index = 0; index < 9; index++)
		{
			tempMatrix[index] = aMatrix0[index] - aMatrix1[index];
		}

		return tempMatrix;
	}

	template <class T>
	Matrix3x3<T> operator-=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		return aMatrix0 = aMatrix0 - aMatrix1;
	}

	template <class T>
	Matrix3x3<T> operator*(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		Matrix3x3<T> tempMatrix;

		tempMatrix(1, 1) =
			aMatrix0(1, 1) * aMatrix1(1, 1) +
			aMatrix0(1, 2) * aMatrix1(2, 1) +
			aMatrix0(1, 3) * aMatrix1(3, 1);

		tempMatrix(1, 2) =
			aMatrix0(1, 1) * aMatrix1(1, 2) +
			aMatrix0(1, 2) * aMatrix1(2, 2) +
			aMatrix0(1, 3) * aMatrix1(3, 2);

		tempMatrix(1, 3) =
			aMatrix0(1, 1) * aMatrix1(1, 3) +
			aMatrix0(1, 2) * aMatrix1(2, 3) +
			aMatrix0(1, 3) * aMatrix1(3, 3);


		tempMatrix(2, 1) =
			aMatrix0(2, 1) * aMatrix1(1, 1) +
			aMatrix0(2, 2) * aMatrix1(2, 1) +
			aMatrix0(2, 3) * aMatrix1(3, 1);

		tempMatrix(2, 2) =
			aMatrix0(2, 1) * aMatrix1(1, 2) +
			aMatrix0(2, 2) * aMatrix1(2, 2) +
			aMatrix0(2, 3) * aMatrix1(3, 2);

		tempMatrix(2, 3) =
			aMatrix0(2, 1) * aMatrix1(1, 3) +
			aMatrix0(2, 2) * aMatrix1(2, 3) +
			aMatrix0(2, 3) * aMatrix1(3, 3);


		tempMatrix(3, 1) =
			aMatrix0(3, 1) * aMatrix1(1, 1) +
			aMatrix0(3, 2) * aMatrix1(2, 1) +
			aMatrix0(3, 3) * aMatrix1(3, 1);

		tempMatrix(3, 2) =
			aMatrix0(3, 1) * aMatrix1(1, 2) +
			aMatrix0(3, 2) * aMatrix1(2, 2) +
			aMatrix0(3, 3) * aMatrix1(3, 2);

		tempMatrix(3, 3) =
			aMatrix0(3, 1) * aMatrix1(1, 3) +
			aMatrix0(3, 2) * aMatrix1(2, 3) +
			aMatrix0(3, 3) * aMatrix1(3, 3);

		return tempMatrix;
	}

	template <class T>
	Matrix3x3<T> operator*=(Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		return aMatrix0 = aMatrix0 * aMatrix1;
	}

	template <class T>
	Vector3<T> operator*(const Matrix3x3<T>& aMatrix, const Vector3<T>& aVector)
	{
		Vector3<T> tempVector;

		tempVector.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1);
		tempVector.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2);
		tempVector.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3);

		return tempVector;
	}

	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix3x3<T>& aMatrix)
	{
		return aMatrix * aVector;
	}

	template <class T>
	bool operator==(const Matrix3x3<T>& aMatrix0, const Matrix3x3<T>& aMatrix1)
	{
		for (int index = 0; index < 9; index++)
		{
			if (aMatrix0[index] != aMatrix1[index])
			{
				return false;
			}
		}

		return true;
	}


	template<class T>
	inline Matrix3x3<T>::Matrix3x3()
	{
		myArray =
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		for (int index = 0; index < 9; index++)
		{
			myArray[index] = aMatrix[index];
		}
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		myArray[0] = aMatrix[0];
		myArray[1] = aMatrix[1];
		myArray[2] = aMatrix[2];

		myArray[3] = aMatrix[4];
		myArray[4] = aMatrix[5];
		myArray[5] = aMatrix[6];

		myArray[6] = aMatrix[8];
		myArray[7] = aMatrix[9];
		myArray[8] = aMatrix[10];
	}

	template<class T>
	inline Matrix3x3<T>::Matrix3x3(std::initializer_list<T> aList)
	{
		for (int index = 0; index < 9; index++)
		{
			myArray[index] = *(aList.begin() + index);
		}
	}

	template<class T>
	inline T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		return myArray[(aRow - 1) * 3 + (aColumn - 1)];
	}

	template<class T>
	inline const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		return myArray[(aRow - 1) * 3 + (aColumn - 1)];
	}

	template<class T>
	inline T& Matrix3x3<T>::operator[](const int aIndex)
	{
		return myArray[aIndex];
	}

	template<class T>
	inline const T& Matrix3x3<T>::operator[](const int aIndex) const
	{
		return myArray[aIndex];
	}

	template<class T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix3x3<T>& aMatrix)
	{
		for (int index = 0; index < 9; index++)
		{
			myArray[index] = aMatrix[index];
		}

		return *this;
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		const T cos = std::cos(aAngleInRadians);
		const T sin = std::sin(aAngleInRadians);

		return
		{
			T(1), 0, 0,
			0, cos, sin,
			0, -sin, cos
		};
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		const T cos = std::cos(aAngleInRadians);
		const T sin = std::sin(aAngleInRadians);

		return
		{
			cos, 0, -sin,
			0, T(1), 0,
			sin, 0, cos
		};
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		const T cos = std::cos(aAngleInRadians);
		const T sin = std::sin(aAngleInRadians);

		return
		{
			cos, sin, 0,
			-sin, cos, 0,
			0, 0, T(1)
		};
	}

	template<class T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> tempMatrix;

		for (int row = 0; row < 3; row++)
		{
			for (int column = 0; column < 3; column++)
			{
				tempMatrix[row + column * 3] = aMatrixToTranspose[column + row * 3];
			}
		}

		return tempMatrix;
	}
}