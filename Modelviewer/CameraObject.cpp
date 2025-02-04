#include "CameraObject.h"

CameraObject::CameraObject(std::shared_ptr<InputHandler>& anInputHandler, Vector3f& aPosition, Vector3f& aRotation, float aSpeed)
{
	myInputHandler = anInputHandler;
	myPosition = Vector4f(aPosition.x, aPosition.y, aPosition.z, 1);
	myShouldDisable = false;
	myData.Transform.SetPosition(aPosition);
	myData.Transform.SetRotation(aRotation);
	Matrix4f m = myData.Transform.GetMatrix();
	float myScreenHeight = 900.0f;
	float myScreenWidth = 1600.0f;
	float myFar = 10000.0f;
	float myNear = 1.0f;
	float myFOV = 1.3962634f;

	myData.Speed = aSpeed;

	myProjectionMatrix = CommonUtilities::Matrix4x4<float>();
	myProjectionMatrix(1, 1) = (1 / tan(myFOV / 2));
	myProjectionMatrix(2, 2) = (myScreenWidth / myScreenHeight) * (1 / tan(myFOV / 2));
	myProjectionMatrix(3, 3) = (myFar / (myFar - myNear));
	myProjectionMatrix(3, 4) = 1;
	myProjectionMatrix(4, 3) = (((myNear * myFar) * -1) / (myFar - myNear));
	myProjectionMatrix(4, 4) = 0;
}

CameraObject::~CameraObject()
{
}

void CameraObject::Update(float aDeltaTime)
{
	if (!myShouldDisable)
	{
		if (myInputHandler->IsKeyPressed('W') && !myInputHandler->IsKeyPressed(VK_CONTROL))
		{
			Vector4f newPos = myData.Transform.GetPosition();
			newPos += (myData.Speed * aDeltaTime * myDirection.forward) * myData.Transform.GetMatrix();
			myData.Transform.SetPosition(newPos);
		}
		else if (myInputHandler->IsKeyPressed('S') && !myInputHandler->IsKeyPressed(VK_CONTROL))
		{
			Vector4f newPos = myData.Transform.GetPosition();
			newPos += (myData.Speed * aDeltaTime * myDirection.backward) * myData.Transform.GetMatrix();
			myData.Transform.SetPosition(newPos);
		}
		else if (myInputHandler->IsKeyPressed('A') && !myInputHandler->IsKeyPressed(VK_CONTROL))
		{
			Vector4f newPos = myData.Transform.GetPosition();
			newPos += (myData.Speed * aDeltaTime * myDirection.left) * myData.Transform.GetMatrix();
			myData.Transform.SetPosition(newPos);
		}
		else if (myInputHandler->IsKeyPressed('D') && !myInputHandler->IsKeyPressed(VK_CONTROL))
		{
			Vector4f newPos = myData.Transform.GetPosition();
			newPos += (myData.Speed * aDeltaTime * myDirection.right) * myData.Transform.GetMatrix();
			myData.Transform.SetPosition(newPos);
		}
		else if (myInputHandler->IsKeyPressed('Z') && !myInputHandler->IsKeyPressed(VK_CONTROL))
		{
			Vector4f newPos = myData.Transform.GetPosition();
			newPos += (myData.Speed * aDeltaTime * myDirection.down) * myData.Transform.GetMatrix();
			myData.Transform.SetPosition(newPos);
		}
		else if (myInputHandler->IsKeyPressed('X') && !myInputHandler->IsKeyPressed(VK_CONTROL))
		{
			Vector4f newPos = myData.Transform.GetPosition();
			newPos += (myData.Speed * aDeltaTime * myDirection.up) * myData.Transform.GetMatrix();
			myData.Transform.SetPosition(newPos);
		}

		if (myInputHandler->IsKeyPressed('Q') && !myInputHandler->IsKeyPressed(VK_CONTROL))
		{
			myData.Transform.Rotate(Vector3f( 0.f,-aDeltaTime * myData.Speed / 2,0.f ));
		}
		else if (myInputHandler->IsKeyPressed('E') && !myInputHandler->IsKeyPressed(VK_CONTROL))
		{
			myData.Transform.Rotate(Vector3f( 0.f,aDeltaTime * myData.Speed / 2,0.f ));
		}
	}
	else
	{
		return;
	}
}

CommonUtilities::Matrix4x4<float>& CameraObject::GetProjection()
{
	return myProjectionMatrix;
}

const CommonUtilities::Vector3f& CameraObject::GetPosition()
{
	return myData.Transform.GetPosition();
}

//const float& CameraObject::GetCameraSpeed() const
//{
//	return mySpeed;
//}
//
//void CameraObject::SetCameraSpeed(const float& aSpeed)
//{
//	mySpeed = aSpeed;
//}

Transform& CameraObject::GetTransform()
{
	return myData.Transform;
}

void CameraObject::SetTransform(const Vector3f aPosition)
{
	myData.Transform.SetPosition(aPosition);
}

void CameraObject::SetTransform(const Transform& aTransform)
{
	myData.Transform = aTransform;
}

void CameraObject::DisableCameraMovement(bool aShouldDisable)
{
	myShouldDisable = aShouldDisable;
}
