#include "LightObject.h"

LightObject::LightObject(CommonUtilities::Vector3f aWorldCenter, float aWorldRadius, CommonUtilities::Vector4f aColor, CommonUtilities::Vector3f aDirection, float anIntensity)
{
	//myData.Color = aColor;
	myData.Color = aColor;
	myData.Direction = aDirection;
	myData.Intensity = anIntensity;
	myPosition = -2.0f * aWorldRadius * myData.Direction;
	CommonUtilities::Vector3f targetPos = aWorldCenter;
	CommonUtilities::Vector3f globalUpDir = { 0, 1, 0 };
	myView = CommonUtilities::Matrix4x4<float>::CreateLookAtMatrix(myPosition, targetPos, globalUpDir);
	CommonUtilities::Vector4f frustrumCenter = CommonUtilities::Vector4f(targetPos.x, targetPos.y, targetPos.z, 0) * myView;
	float leftPlane = frustrumCenter.x - aWorldRadius;
	float bottomPlane = frustrumCenter.y - aWorldRadius;
	float nearPlane = 1;
	float rightPlane = frustrumCenter.x + aWorldRadius;
	float topPlane = frustrumCenter.y + aWorldRadius;
	float farPlane = aWorldRadius * 4;

	myProj = CommonUtilities::Matrix4x4<float>::CreateOrthographicProjection(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
}

LightObject::LightObject(CommonUtilities::Vector4f aColor, CommonUtilities::Vector3f aPosition, float anIntensity, float aRange)
{
	myData.Color = aColor;
	myData.Transform.SetPosition(aPosition);
	myData.Intensity = anIntensity;
	myData.Range = aRange;
}

LightObject::LightObject(CommonUtilities::Vector4f aColor, CommonUtilities::Vector3f aPosition, CommonUtilities::Vector3f aDirection, float anIntensity, float aRange, float aConeRadius, float aPenumbra)
{
	myData.Color = aColor;
	myData.Transform.SetPosition(aPosition);
	myData.Transform.SetRotation(aDirection);
	myData.Intensity = anIntensity;
	myData.Range = aRange;
	myData.Cone = aConeRadius;
	myData.Penumbra = aPenumbra;
}
//
//void LightObject::SetColor(float aR, float aG, float aB, float anA)
//{
//	myData.Color = { aR,aG,aB,anA };
//}

CommonUtilities::Matrix4x4<float> LightObject::GetViewProjection()
{
	return myView;
}

CommonUtilities::Matrix4x4<float> LightObject::GetLightProjection()
{
	return myProj;
}
