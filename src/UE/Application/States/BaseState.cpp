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
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Uexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Uexpected: handleAttachReject");
}

void BaseState::handleDisconnect()
{
    logger.logError("Uexpected: handleDisconnect");
}

void BaseState::handleCallMessage(common::MessageId msgId)
{
    logger.logError("Uexpected: handleCallMessage: ", msgId);
}

void BaseState::handleCallAccept(common::MessageId msgId)
{
    logger.logError("Uexpected: handleCallAccept: ", msgId);
}

void BaseState::handleCallDropped(common::MessageId msgId)
{
    logger.logError("Uexpected: handleCallDropped: ", msgId);
}

void BaseState::handleSms(common::PhoneNumber from, std::string message)
{
    logger.logError("Uexpected: handleSms: ", from, ", message: ", message);
}
}
