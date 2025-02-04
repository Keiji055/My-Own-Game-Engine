#pragma once
#include <variant>
#include "MessageType.h"

namespace CommonUtilities
{
	class Message
	{
	public:
		Message() = default;
		Message(MessageType aMessageType);
		~Message() = default;
		const MessageType& GetMessageType() const;
	private:
		MessageType myMessageType;
	};
}

