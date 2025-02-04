#include "TransformCommand.h"

TransformCommand::TransformCommand(std::shared_ptr<GameObject>& aGameObject, CommonUtilities::Transform aFrom, CommonUtilities::Transform aTo)
{
	myGameObject = aGameObject;
	myFrom = aFrom;
	myTo = aTo;
}

TransformCommand::~TransformCommand()
{

}

bool TransformCommand::Execute()
{
	if (myGameObject != nullptr)
	{
		myGameObject->SetTransform(myTo);
		return true;
	}
	return false;
}

bool TransformCommand::Undo()
{
	if (myGameObject != nullptr)
	{
		myGameObject->SetTransform(myFrom);
		return true;
	}
	return false;
}
