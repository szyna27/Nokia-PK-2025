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

void BaseState::handleCallTalk(common::PhoneNumber from)
{
    logger.logError("Unexpected: handleCallTalk: ", from);
}

void BaseState::handleCallAccept(common::PhoneNumber from)
{
    logger.logError("Unexpected: handleCallAccept: ", from);
}

void BaseState::handleCallDropped(common::PhoneNumber from)
{
    logger.logError("Unexpected: handleCallDropped: ", from);
}

void BaseState::handleAccept()
{
    logger.logError("Unexpected: handleAccept");
}

void BaseState::handleSMS(common::PhoneNumber from, const std::string &message)
{
    logger.logError("Unexpected: handleSMS: ", from, " ", message);
}

void BaseState::handleCallRequest(common::PhoneNumber from)
{
    logger.logError("Unexpected: handleCallRequest: ", from);
}

void BaseState::handleUIAction(std::optional<std::size_t> selectedIndex){
    logger.logError("Unexpected: handleUiAction(), index: ", (selectedIndex.has_value() ? std::to_string(selectedIndex.value()) : "none"));
}

void BaseState::handleUIBack(){
    logger.logError("Unexpected: handleUiBack()");
}

} // namespace ue
