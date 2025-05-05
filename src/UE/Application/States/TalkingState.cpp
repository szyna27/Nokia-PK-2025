#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context)
    : BaseState(context, "TalkingState")
{
    context.user.showTalking();
    context.user.setHomeCallback([this] { returnToConnectedState(); });
    logger.logDebug("Entering TalkingState");
}

void TalkingState::returnToConnectedState()
{
    logger.logDebug("Returning to ConnectedState");
    context.setState<ConnectedState>();
}
}