#pragma once

#include "Vector.hpp"
#include "Matrix4x4.hpp"
#include <memory>/*
#include "tge/math/Transform.h"*/

namespace CommonUtilities
{
	class Transform
	{
	public:
		Transform();/*
		Transform(Tga::Transform aTransform);*/
		Transform(Vector3f aPosition, Vector3f aRotation, Vector3f aScale);
		Transform(float x, float y, float z, float xr, float yr, float zr, float xs, float ys, float zs);
		Transform(const Transform& anOther);
		~Transform() = default;
		void Init(Vector3f aPosition, Vector3f aRotation, Vector3f aScale);

		//operator Tga::Transform() const;

		void SetPosition(const Vector3f& aPosition);
		void SetRotation(const Vector3f& aDegrees);
		void SetScale(const Vector3f& aScale);

		void Move(const Vector3f& aDirection);
		void Rotate(const Vector3f& aDeltaDegrees);
		void LookAt(const Vector3f& aPosition);

		Matrix4f GetMatrix();
		Matrix4f GetMatrix() const;
		Vector3f GetPosition() const;
		Vector3f GetRotation() const;
		Vector3f GetScale() const;

		Vector3f GetForward() const;
		Vector3f GetRight() const;
		Vector3f GetUp() const;

		Transform& operator=(const Transform& anOther);

	private:
		Matrix4x4<float> myMatrix;

		Vector3f myPosition;
		Vector3f myRotation;
		Vector3f myScale;

		bool IsDirty;
	};


}