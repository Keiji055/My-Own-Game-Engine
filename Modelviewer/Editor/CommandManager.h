#pragma once
#include"AbstractCommand.h"
#include <memory>
#include <stack>
class CommandManager
{
public:

	static void DoCommand(std::shared_ptr<AbstractCommand> aCommand);
	static void Undo();
	static void Redo();
private:

	inline static std::stack<std::shared_ptr<AbstractCommand>> myUndoStack;
	inline static std::stack<std::shared_ptr<AbstractCommand>> myRedoStack;
};