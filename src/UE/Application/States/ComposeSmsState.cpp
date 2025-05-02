#include "ComposeSmsState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

ComposeSmsState::ComposeSmsState(Context &context)
    : BaseState(context, "ComposeSmsState")
{
    logger.logInfo("Entering ComposeSmsState");
    context.user.showComposeSms();
    context.user.setHomeCallback([this] { returnToConnectedState(); });
}

void ComposeSmsState::returnToConnectedState()
{
    logger.logInfo("Returning to ConnectedState");
    context.setState<ConnectedState>();
}
}