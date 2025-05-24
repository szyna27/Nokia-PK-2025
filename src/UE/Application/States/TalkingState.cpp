#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, PhoneNumber from)
    : BaseState(context, "TalkingState"), peerNumber(from)
{
    context.user.showTalking();
    context.user.setHomeCallback([this] { returnToConnectedState(); });
    logger.logDebug("Entering TalkingState");
    context.user.setAcceptCallback([this] { sendCallTalk(); });
    context.user.setRejectCallback([this] { callDropped(); });
}

void TalkingState::returnToConnectedState()
{
    logger.logDebug("Returning to ConnectedState");
    context.setState<ConnectedState>();
}

void TalkingState::callDropped()
{
    logger.logDebug("Dropping call");
    context.bts.sendCallDrop(peerNumber);
    context.setState<ConnectedState>(); 
}

void TalkingState::handleCallDropped(PhoneNumber from)
{
    logger.logDebug("Received call dropped from: ", from);
    context.setState<ConnectedState>();
}

void TalkingState::sendCallTalk()
{
    IUeGui::ICallMode& callMode = context.user.getCallMode();
    auto message = callMode.getOutgoingText();

    if(message.empty()){
        logger.logDebug("Empty message, not sending call talk");
        return;
    }

    logger.logDebug("Sending call talk to: ", peerNumber);

    context.timer.stopTimer();
    context.timer.startTimer(std::chrono::milliseconds(30000));

    callMode.clearOutgoingText();
    context.bts.sendCallTalk(peerNumber, message);
}

void TalkingState::handleCallTalk(const std::string message)
{
    IUeGui::ICallMode& callMode = context.user.getCallMode();
    
    logger.logDebug("Handling call talk from: ", peerNumber, " with message: ", message);

    context.timer.stopTimer();
    context.timer.startTimer(std::chrono::milliseconds(30000));

    callMode.clearIncomingText();
    callMode.appendIncomingText(message);

}

}