#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Unexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Unexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Unexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Unexpected: handleAttachReject");
}

void BaseState::handleDisconnect()
{
    logger.logError("Unexpected: handleDisconnect");
}

void BaseState::handleCallMessage(common::MessageId msgId)
{
    logger.logError("Unexpected: handleCallMessage: ", msgId);
}

void BaseState::handleCallAccept(common::MessageId msgId)
{
    logger.logError("Unexpected: handleCallAccept: ", msgId);
}

void BaseState::handleCallDropped(common::MessageId msgId)
{
    logger.logError("Unexpected: handleCallDropped: ", msgId);
}

void BaseState::handleAccept()
{
    logger.logError("Unexpected: handleAccept");
}

}
