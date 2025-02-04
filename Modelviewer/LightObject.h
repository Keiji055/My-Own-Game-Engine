#pragma once
#include "GameObject.h"
#include <CommonUtilities/Vector.hpp>
#include <CommonUtilities/Matrix4x4.hpp>
using namespace CommonUtilities;

class LightObject : public GameObject
{
public:
	LightObject() = default;
	LightObject(Vector3f aWorldCenter, float aWorldRadius, Vector4f aColor, Vector3f aDirection, float anIntensity);
	LightObject(Vector4f aColor, Vector3f aPosition, float anIntensity, float aRange);
	LightObject(Vector4f aColor, Vector3f aPosition, Vector3f aDirection, float anIntensity, float aRange, float aConeRadius, float aPenumbra);

	//void SetColor(float aR, float aG, float aB, float anA);
	Matrix4x4<float> GetViewProjection();
	Matrix4x4<float> GetLightProjection();

private:
	Vector3f  myPosition;
	Matrix4x4<float> myView;
	Matrix4x4<float> myProj;
};