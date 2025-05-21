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
    context.user.setAcceptCallback([this] { nullptr; });
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
}