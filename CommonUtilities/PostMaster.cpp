#include "PostMaster.h"
#include "PostMaster.h"
namespace CommonUtilities
{
	void PostMaster::Subscribe(const MessageType aMsgType, Observer* aObserver)
	{
		myObservers[aMsgType].push_back(aObserver);
	}

	void PostMaster::SendMessage(const Message& aMessage)
	{
		for (int index = 0; index < myObservers[aMessage.GetMessageType()].size(); index++)
		{
			myObservers[aMessage.GetMessageType()][index]->Recieve(aMessage);
		}
	}

}