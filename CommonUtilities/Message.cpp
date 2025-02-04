#include "Message.h"
namespace CommonUtilities
{
    Message::Message(MessageType aMessageType)
    {
        myMessageType = aMessageType;
    }

    const MessageType& Message::GetMessageType() const
    {
        return myMessageType;
    }
}
