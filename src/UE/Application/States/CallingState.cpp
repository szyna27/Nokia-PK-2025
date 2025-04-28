#include "CallingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include <chrono>

using namespace std::chrono;

namespace ue
{
    CallingState::CallingState(Context &context) : BaseState(context, "Call state")
    {
        logger.logInfo("Call state");
        context.timer.startTimer(60s);
    }

    void CallingState::handleTimeout()
    {
        logger.logInfo("Call timeout");
        context.user.showAlert("Call timeout", "Recipient did not answer");
    }

    void CallingState::handleCallDropped()
    {
        logger.logInfo("Call dropped by BTS");
        context.user.showAlert("Call dropped", "Recipient drop the call");
        context.setState<ConnectedState>();
    }

    void CallingState::handleDisconnect()
    {
        logger.logError("Call disconnected");
        context.setState<NotConnectedState>();
    }

    void CallingState::handleUIEnding()
    {
        logger.logInfo("Call was cancelled");
        // TODO: context.bts.sendCallEnding()
        context.setState<ConnectedState>();
    }

    void CallingState::handleUICalling(std::optional<std::size_t> selectedIndex)
    {
        common::PhoneNumber recipientNumber;
        logger.logInfo("Calling number ", recipientNumber);

        if (!recipientNumber.isValid())
        {
            logger.logError("Worng phone number");
            context.user.showAlert("Invalid recipient number", "Enter valid phone number");
            return;
        }

        context.bts.sendCallRequest(recipientNumber);
    }

    void CallingState::handleCallAccepted(common::PhoneNumber recipient)
    {
        logger.logInfo("Call picked up by: ", recipient);
        // TODO: context.setState<TalkingState>();
    }

    void CallingState::handleUnknownRecipient(common::PhoneNumber recipient)
    {
        logger.logInfo("Unknown recipient: ", recipient);
        context.user.showAlert("Failed", "Number does not exists.");
        context.setState<ConnectedState>();
    }
}