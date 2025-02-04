#pragma once
#include <cassert>
#include <string.h>
#include <math.h>
#include "Vector4.hpp"


namespace CommonUtilities
{
	template <class T>
	class Vector3;

	template <class T>
	class Matrix4x4
	{
	public:
		Matrix4x4();
		Matrix4x4(const T& aValue);
		Matrix4x4(const Matrix4x4<T>& aMatrix);
		Matrix4x4(const Vector4<T>& aRowOne, const Vector4<T>& aRowTwo, const Vector4<T>& aRowThree, const Vector4<T>& aRowFour);

		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		T& operator[](const int anElement);
		const T& operator[](const int anElement) const;

		Matrix4x4<T>& operator=(const Matrix4x4<T>& anOther);
		Matrix4x4<T>& operator+=(const Matrix4x4<T>& anOther);
		Matrix4x4<T>& operator-=(const Matrix4x4<T>& anOther);
		Matrix4x4<T>& operator*=(const Matrix4x4<T>& anOther);
		Matrix4x4<T>& operator*=(const T& aScalar);

		bool operator==(const Matrix4x4<T>& anOther) const;
		bool operator!=(const Matrix4x4<T>& anOther) const;

		static Matrix4x4<T> CreateRotationAroundX(const T anAngleInDegrees);
		static Matrix4x4<T> CreateRotationAroundY(const T anAngleInDegrees);
		static Matrix4x4<T> CreateRotationAroundZ(const T anAngleInDegrees);
		static Matrix4x4<T> CreateRotationMatrix(const Vector3<T>& anAngleInDegrees);
		static Matrix4x4<T> CreateTranslationMatrix(const Vector3<T>& aDirection);
		static Matrix4x4<T> CreateTranslationMatrix(const Vector4<T>& aDirection);
		static Matrix4x4<T> CreateScaleMatrix(const Vector3<T>& aScale);
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrix);
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aMatrix);
		static Matrix4x4<T> CreateOrthographicProjection(T aLeftPlane, T aRightPlane, T aBottomPlane, T aTopPlane, T aNearPlane, T aFarPlane);
		static Matrix4x4<T> CreateLookAtMatrix(const Vector3<T>& aFrom, const Vector3<T>& aTarget, const Vector3<T>& anUp);

		static Matrix4x4<T> Identity;

	private:

		union
		{
			T myElements[16];
			Vector4<T> myRows[4];
		};

	};

	typedef Matrix4x4<float> Matrix4f;

	template<class T>
	Matrix4x4<T>::Matrix4x4() :
		myElements
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	}
	{
	}

	template<class T>
	Matrix4x4<T>::Matrix4x4(const T& aValue) : myElements{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T }
	{
	}

	template<class T>
	Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix) :
		myElements{
			aMatrix.myElements[0],
			aMatrix.myElements[1],
			aMatrix.myElements[2],
			aMatrix.myElements[3],
			aMatrix.myElements[4],
			aMatrix.myElements[5],
			aMatrix.myElements[6],
			aMatrix.myElements[7],
			aMatrix.myElements[8],
			aMatrix.myElements[9],
			aMatrix.myElements[10],
			aMatrix.myElements[11],
			aMatrix.myElements[12],
			aMatrix.myElements[13],
			aMatrix.myElements[14],
			aMatrix.myElements[15]
	}
	{
	}

	template <class T>
	Matrix4x4<T>::Matrix4x4(const Vector4<T>& aRowOne, const Vector4<T>& aRowTwo, const Vector4<T>& aRowThree, const Vector4<T>& aRowFour) :
		myRows
	{
		aRowOne,
		aRowTwo,
		aRowThree,
		aRowFour
	}
	{
	}

	template <class T>
	T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert(!(aRow > 4 || aColumn > 4 || aRow <= 0 || aColumn <= 0));
		return myElements[(aRow - 1) * 4 + aColumn - 1];
	}

	template<class T>
	const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(!(aRow > 4 || aColumn > 4 || aRow <= 0 || aColumn <= 0));
		return myElements[(aRow - 1) * 4 + aColumn - 1];
	}

	template <class T>
	T& Matrix4x4<T>::operator[](const int anElement)
	{
		assert(anElement < 16 && anElement >= 0);
		return myElements[anElement];
	}

	template<class T>
	const T& Matrix4x4<T>::operator[](const int anElement) const
	{
		assert(anElement < 16 && anElement >= 0);
		return myElements[anElement];
	}

	template <class T>
	Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& anOther)
	{
		memcpy(myElements, anOther.myElements, sizeof(T) * 16);

		return *this;
	}

	template <class T>
	Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& anOther)
	{
		for (int index = 0; index < 16; index++)
		{
			myElements[index] += anOther.myElements[index];
		}

		return *this;
	}

	template <class T>
	Matrix4x4<T>& Matrix4x4<T>::operator-=(const Matrix4x4<T>& anOther)
	{
		for (int index = 0; index < 16; index++)
		{
			myElements[index] -= anOther.myElements[index];
		}

		return *this;
	}

	template <class T>
	Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& anOther)
	{
		Vector4<T> otherColumnVector;
		T newData[16];

		for (int column = 0; column < 4; column++)
		{
			otherColumnVector = { anOther[column], anOther[column + 4], anOther[column + 8], anOther[column + 12] };
			for (int row = 0; row < 4; row++)
			{
				newData[row * 4 + column] = myRows[row].Dot(otherColumnVector);
			}
		}

		memcpy(myElements, newData, sizeof(T) * 16);
		return *this;
	}

	template<class T>
	Matrix4x4<T>& Matrix4x4<T>::operator*=(const T& aScalar)
	{
		for (int index = 0; index < 16; index++)
		{
			myElements[index] *= aScalar;
		}

		return *this;
	}

	template <class T>
	bool Matrix4x4<T>::operator==(const Matrix4x4<T>& anOther) const
	{
		for (int index = 0; index < 16; index++)
		{
			if (myElements[index] != anOther[index])
			{
				return false;
			}
		}
		return true;
	}

	template <class T>
	bool Matrix4x4<T>::operator!=(const Matrix4x4<T>& anOther) const
	{
		return !((*this) == anOther);
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T anAngleInDegrees)
	{
		float radians = anAngleInDegrees * 0.0174532925f;

		Matrix4x4<T> matrix;
		matrix(2, 2) = static_cast<T>(cos(radians));
		matrix(2, 3) = static_cast<T>(sin(radians));

		matrix(3, 2) = static_cast<T>(-sin(radians));
		matrix(3, 3) = static_cast<T>(cos(radians));

		return matrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const T anAngleInDegrees)
	{
		float radians = anAngleInDegrees * 0.0174532925f;

		Matrix4x4<T> matrix;
		matrix(1, 1) = static_cast<T>(cos(radians));
		matrix(1, 3) = static_cast<T>(-sin(radians));

		matrix(3, 1) = static_cast<T>(sin(radians));
		matrix(3, 3) = static_cast<T>(cos(radians));

		return matrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const T anAngleInDegrees)
	{
		float radians = anAngleInDegrees * 0.0174532925f;

		Matrix4x4<T> matrix;
		matrix(1, 1) = static_cast<T>(cos(radians));
		matrix(1, 2) = static_cast<T>(sin(radians));

		matrix(2, 1) = static_cast<T>(-sin(radians));
		matrix(2, 2) = static_cast<T>(cos(radians));

		return matrix;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationMatrix(const Vector3<T>& anAngleInDegrees)
	{
		Matrix4x4<T> xmatrix = Matrix4x4<T>::CreateRotationAroundX(anAngleInDegrees.x);
		Matrix4x4<T> ymatrix = Matrix4x4<T>::CreateRotationAroundY(anAngleInDegrees.y);
		Matrix4x4<T> zmatrix = Matrix4x4<T>::CreateRotationAroundZ(anAngleInDegrees.z);

		return xmatrix * ymatrix * zmatrix;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::CreateTranslationMatrix(const Vector3<T>& aDirection)
	{
		Matrix4x4<T> matrix;
		matrix(4, 1) = aDirection.x;
		matrix(4, 2) = aDirection.y;
		matrix(4, 3) = aDirection.z;

		return matrix;
	}

	template<class T>
	Matrix4x4<T> Matrix4x4<T>::CreateTranslationMatrix(const Vector4<T>& aDirection)
	{
		return CreateTranslationMatrix(Vector3<T>(aDirection.x, aDirection.y, aDirection.z));
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateScaleMatrix(const Vector3<T>& aScale)
	{
		Matrix4x4<T> matrix;
		matrix(1, 1) = aScale.x;
		matrix(2, 2) = aScale.y;
		matrix(3, 3) = aScale.z;
		return matrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> matrix(aMatrix);
		matrix(2, 1) = aMatrix(1, 2);
		matrix(1, 2) = aMatrix(2, 1);

		matrix(3, 1) = aMatrix(1, 3);
		matrix(1, 3) = aMatrix(3, 1);

		matrix(4, 1) = aMatrix(1, 4);
		matrix(1, 4) = aMatrix(4, 1);

		matrix(3, 2) = aMatrix(2, 3);
		matrix(2, 3) = aMatrix(3, 2);

		matrix(4, 2) = aMatrix(2, 4);
		matrix(2, 4) = aMatrix(4, 2);

		matrix(4, 3) = aMatrix(3, 4);
		matrix(3, 4) = aMatrix(4, 3);

		return matrix;
	}


	template <class T>
	Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> rMatrix;

		rMatrix(2, 1) = aMatrix(1, 2);
		rMatrix(1, 2) = aMatrix(2, 1);

		rMatrix(3, 1) = aMatrix(1, 3);
		rMatrix(1, 3) = aMatrix(3, 1);

		rMatrix(3, 2) = aMatrix(2, 3);
		rMatrix(2, 3) = aMatrix(3, 2);

		rMatrix(1, 1) = aMatrix(1, 1);
		rMatrix(2, 2) = aMatrix(2, 2);
		rMatrix(3, 3) = aMatrix(3, 3);

		Matrix4x4<T> tMatrix;
		tMatrix(4, 1) = -aMatrix(4, 1);
		tMatrix(4, 2) = -aMatrix(4, 2);
		tMatrix(4, 3) = -aMatrix(4, 3);

		rMatrix = tMatrix * rMatrix;

		return rMatrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateOrthographicProjection(T aLeftPlane, T aRightPlane, T aBottomPlane, T aTopPlane, T aNearPlane, T aFarPlane)
	{
		const T reciprocalWidth = T(1.0) / (aRightPlane - aLeftPlane);
		const T reciprocalHeight = T(1.0) / (aTopPlane - aBottomPlane);
		const float fRange = T(1.0) / (aFarPlane - aNearPlane);

		Matrix4x4<T> result;

		result(1, 1) = reciprocalWidth + reciprocalWidth;
		result(2, 2) = reciprocalHeight + reciprocalHeight;
		result(3, 3) = fRange;
		result(4, 1) = -(aLeftPlane + aRightPlane) * reciprocalWidth;
		result(4, 2) = -(aTopPlane + aBottomPlane) * reciprocalHeight;
		result(4, 3) = -fRange * aNearPlane;
		result(4, 4) = 1.0f;
		return result;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateLookAtMatrix(const Vector3<T>& aFrom, const Vector3<T>& aTarget, const Vector3<T>& anUp)
	{
		Vector3<T> forward = (aTarget - aFrom).GetNormalized();
		Vector3<T> right = anUp.Cross(forward).GetNormalized();
		Vector3<T> up = forward.Cross(right).GetNormalized();

		Matrix4x4<T> result;

		result(1, 1) = right.x;
		result(2, 1) = right.y;
		result(3, 1) = right.z;

		result(1, 2) = up.x;
		result(2, 2) = up.y;
		result(3, 2) = up.z;

		result(1, 3) = forward.x;
		result(2, 3) = forward.y;
		result(3, 3) = forward.z;

		result(1, 4) = aFrom.x;
		result(2, 4) = aFrom.y;
		result(3, 4) = aFrom.z;

		return result;
	}

	template <class T>
	Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrixLeft, const Matrix4x4<T>& aMatrixRight)
	{
		Matrix4x4<T> matrix(aMatrixLeft);
		matrix += aMatrixRight;
		return matrix;
	}

	template <class T>
	Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrixLeft, const Matrix4x4<T>& aMatrixRight)
	{
		Matrix4x4<T> matrix(aMatrixLeft);
		matrix -= aMatrixRight;
		return matrix;
	}

	template <class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrixLeft, const Matrix4x4<T>& aMatrixRight)
	{
		Matrix4x4<T> matrix(aMatrixLeft);
		matrix *= aMatrixRight;
		return matrix;
	}

	template <class T>
	Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix, const T& aScalar)
	{
		Matrix4x4<T> matrix(aMatrix);
		matrix *= aScalar;
		return matrix;
	}

	template <class T>
	Matrix4x4<T> operator*(const T& aScalar, const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> matrix(aMatrix);
		matrix *= aScalar;
		return matrix;
	}


	template <class T>
	Vector4<T>& operator*=(Vector4<T>& aVector, const Matrix4x4<T> aMatrix)
	{
		Vector4<T> columnVector;
		Vector4<T> originalVector(aVector);

		columnVector = { aMatrix[0], aMatrix[4], aMatrix[8], aMatrix[12] };
		aVector.x = columnVector.Dot(originalVector);

		columnVector = { aMatrix[1], aMatrix[5], aMatrix[9], aMatrix[13] };
		aVector.y = columnVector.Dot(originalVector);

		columnVector = { aMatrix[2], aMatrix[6], aMatrix[10], aMatrix[14] };
		aVector.z = columnVector.Dot(originalVector);

		columnVector = { aMatrix[3], aMatrix[7], aMatrix[11], aMatrix[15] };
		aVector.w = columnVector.Dot(originalVector);

		return aVector;
	}


	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> vector(aVector);
		vector *= aMatrix;
		return vector;
	}

	template <class T>
	Vector4<T> operator*(const Matrix4x4<T>& aMatrix, const Vector4<T>& aVector)
	{
		Vector4<T> vector(aVector);
		vector *= aMatrix;
		return vector;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Identity(Vector4<T>(1, 0, 0, 0), Vector4<T>(0, 1, 0, 0), Vector4<T>(0, 0, 1, 0), Vector4<T>(0, 0, 0, 1));
}