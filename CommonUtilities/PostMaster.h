#pragma once
#include "Observer.h"
#include "Message.h"
#include <vector>
#include <array>
namespace CommonUtilities
{
	class PostMaster
	{
	public:
		static PostMaster& GetInstance()
		{
			static PostMaster instance;

			return instance;
		}
		void Subscribe(const MessageType aMsgType, Observer* aObserver);
		void SendMessage(const Message& aMessage);

	private:
		std::array<std::vector<Observer*>, MessageType::Count> myObservers;
	};
}