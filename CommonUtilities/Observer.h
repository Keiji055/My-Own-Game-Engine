#pragma once
#include "Message.h"

namespace CommonUtilities
{
	class Observer
	{
	public:
		virtual void Recieve(const Message& aMessage) = 0;
	};
}

