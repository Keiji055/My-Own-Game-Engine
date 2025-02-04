#include "Transform.h"

namespace CU = CommonUtilities;

CU::Transform::Transform() :
	myScale(1, 1, 1),
	myPosition(0, 0, 0),
	myRotation(0, 0, 0),
	IsDirty(false),
	myMatrix()
{
}
//
//Transform::Transform(Tga::Transform aTransform) :myPosition(aTransform.GetPosition()), myRotation(aTransform.GetRotation()* FMath::DegToRad), myScale(aTransform.GetScale())
//{
//}

CU::Transform::Transform(Vector3f aPosition, CU::Vector3f aRotation, CU::Vector3f aScale) :
	myPosition(aPosition),
	myRotation(aRotation),
	myScale(aScale),
	IsDirty(true),
	myMatrix()
{
}

CommonUtilities::Transform::Transform(float x, float y, float z, float xr, float yr, float zr, float xs, float ys, float zs)
{
	myPosition = { x, y, z };
	myRotation = { xr, yr, zr };
	myScale = { xs, ys, zs };
	IsDirty = true;
	myMatrix = {};
}

CU::Transform::Transform(const Transform& anOther) :
	myPosition(anOther.myPosition),
	myRotation(anOther.myRotation),
	myScale(anOther.myScale),
	myMatrix(anOther.myMatrix),
	IsDirty(anOther.IsDirty)
{
}

void CU::Transform::Init(CU::Vector3<float> aPosition, CU::Vector3<float> aRotation, CU::Vector3<float> aScale)
{
	myScale = aScale;
	myPosition = aPosition;
	myRotation = aRotation;
}
//
//Transform::operator Tga::Transform() const
//{
//	Tga::Transform result;
//
//	result.SetPosition(GetPosition());
//	result.SetRotation(GetRotation());
//	result.SetScale(GetScale());
//
//	return result;
//}

void CU::Transform::SetPosition(const CU::Vector3<float>& aPosition)
{
	myPosition = aPosition;
	IsDirty = true;
}

void CU::Transform::SetRotation(const CU::Vector3<float>& aDegrees)
{
	myRotation = aDegrees;
	IsDirty = true;
}

void CU::Transform::SetScale(const CU::Vector3f& aScale)
{
	myScale = aScale;
	IsDirty = true;
}

void CU::Transform::Move(const CU::Vector3<float>& aDirection)
{
	myPosition += aDirection;
	IsDirty = true;
}

void CU::Transform::Rotate(const CU::Vector3<float>& aDeltaDegrees)
{
	myRotation += aDeltaDegrees;
	IsDirty = true;
}

void CU::Transform::LookAt(const CU::Vector3f& aPosition)
{
	CU::Vector4f direction(aPosition.x - myPosition.x, aPosition.y - myPosition.y, aPosition.z - myPosition.z, 0);

	if (direction.LengthSqr() <= 0)
	{
		return; //error check typ.
	}
	CU::Vector3f result(myRotation.x, 0, myRotation.z);

	result.y = atan2f(direction.x, direction.z);
	CU::Matrix4f rotMatrix = CU::Matrix4f::CreateRotationAroundY(-result.y);

	direction *= rotMatrix;
	result.x = atan2f(-direction.y, direction.z);

	myRotation = result * 57.2957795f;
	IsDirty = true;
}

CU::Matrix4x4<float> CU::Transform::GetMatrix()
{
	if (IsDirty)
	{
		CU::Matrix4x4<float> result = CU::Matrix4x4<float>::CreateScaleMatrix(myScale);
		result *= CU::Matrix4x4<float>::CreateRotationMatrix(myRotation);
		result(4, 1) = myPosition.x;
		result(4, 2) = myPosition.y;
		result(4, 3) = myPosition.z;

		IsDirty = false;
		myMatrix = result;
		return result;
	}
	else
	{
		return myMatrix;
	}
}

CU::Matrix4x4<float> CU::Transform::GetMatrix() const
{
	CU::Matrix4x4<float> result = CU::Matrix4x4<float>::CreateScaleMatrix(myScale);
	result *= CU::Matrix4x4<float>::CreateRotationMatrix(myRotation);
	result(4, 1) = myPosition.x;
	result(4, 2) = myPosition.y;
	result(4, 3) = myPosition.z;

	return result;
}

CU::Vector3f CU::Transform::GetPosition() const
{
	return myPosition;
}

CU::Vector3f CU::Transform::GetRotation() const
{
	return myRotation;
}

CU::Vector3f CU::Transform::GetScale() const
{
	return myScale;
}

CU::Vector3<float> CU::Transform::GetForward() const
{

	return (CU::Vector4<float>::Forward * GetMatrix()).GetNormalized();
}

CU::Vector3<float> CU::Transform::GetRight() const
{
	return (CU::Vector4<float>::Right * GetMatrix()).GetNormalized();
}

CU::Vector3<float> CU::Transform::GetUp() const
{
	return (CU::Vector4<float>::Up * GetMatrix()).GetNormalized();
}

CU::Transform& CU::Transform::operator=(const Transform& anOther)
{
	myPosition = anOther.myPosition;
	myRotation = anOther.myRotation;
	myScale = anOther.myScale;
	IsDirty = anOther.IsDirty;
	myMatrix = anOther.myMatrix;

	return *this;
}