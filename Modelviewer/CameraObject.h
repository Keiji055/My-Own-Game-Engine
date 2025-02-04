#pragma once
#include "GameObject.h"
#include <CommonUtilities/InputHandler.h>
#include <CommonUtilities/Vector.hpp>
#include <CommonUtilities/Matrix4x4.hpp>
#include <memory>
using namespace CommonUtilities;
struct Direction
{
	Vector4f forward = { 0, 0, 1 ,0};
	Vector4f backward = { 0, 0, -1 ,0};
	Vector4f right = { 1, 0, 0 ,0};
	Vector4f left = { -1, 0, 0,0 };
	Vector4f up = { 0, 1, 0 ,0};
	Vector4f down = { 0, -1, 0 ,0};
};


class CameraObject : public GameObject
{
public:
	CameraObject(std::shared_ptr<InputHandler>& anInputHandler, Vector3f& aPosition, Vector3f& aRotation, float aSpeed);
	~CameraObject();

	void Update(float aDeltaTime);
	Matrix4x4<float>& GetProjection();
	const Vector3f& GetPosition();
	Transform& GetTransform() override;
	void SetTransform(const Vector3f aPosition) override;
	void SetTransform(const Transform& aTransform) override;
	void DisableCameraMovement(bool aShouldDisable);
private:
	bool myShouldDisable;
	Matrix4x4<float> myProjectionMatrix;
	std::shared_ptr<InputHandler> myInputHandler;
	Vector4f myPosition;
	Direction myDirection;
};
