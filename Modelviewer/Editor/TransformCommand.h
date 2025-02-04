#pragma once
#include "AbstractCommand.h"
#include "../GameObject.h"
#include <memory>
#include <CommonUtilities/Transform.h>


class TransformCommand : public AbstractCommand
{
public:
	TransformCommand(std::shared_ptr<GameObject>& aGameObject, CommonUtilities::Transform from, CommonUtilities::Transform to);
	~TransformCommand() override;

	bool Execute() override;
	bool Undo() override;

private:
	std::shared_ptr<GameObject> myGameObject;
	CommonUtilities::Transform myFrom;
	CommonUtilities::Transform myTo;
};