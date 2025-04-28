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

    void BaseState::handleUIEnding()
    {
        // Implementation for handling UI ending
    }

    void BaseState::handleUICalling(std::optional<std::size_t> selectedIndex)
    {
        // Implementation for handling UI calling with optional selected index
    }

    void BaseState::handleCallAccepted(common::PhoneNumber recipient)
    {
        // Implementation for handling accepted call
    }

    void BaseState::handleUnknownRecipient(common::PhoneNumber recipient)
    {
        // Implementation for handling unknown recipient
    }

    void BaseState::handleCallDropped()
    {
        // Implementation for handling dropped call
    }
}
