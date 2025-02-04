#include "CommandManager.h"

void CommandManager::DoCommand(std::shared_ptr<AbstractCommand> aCommand)
{
	aCommand->Execute();
	myUndoStack.push(aCommand);
}

void CommandManager::Undo()
{
	if (myUndoStack.size() > 0)
	{
		myRedoStack.push(myUndoStack.top());
		myUndoStack.top()->Undo();
		myUndoStack.pop();
	}
}

void CommandManager::Redo()
{
	if (myRedoStack.size() > 0)
	{
		myRedoStack.top()->Execute();
		myUndoStack.push(myRedoStack.top());
		myRedoStack.pop();

	}
}
