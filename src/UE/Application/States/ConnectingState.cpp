#include "ConnectingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ViewSMSListState.hpp"
namespace ue
{

ConnectingState::ConnectingState(Context &context)
    : BaseState(context, "ConnectingState")
{
    context.user.showConnecting();
}

void ConnectingState::handleAttachAccept()
{
    context.timer.stopTimer();

    context.setState<ConnectedState>();
}

void ConnectingState::handleAttachReject()
{
    context.timer.stopTimer();

    context.setState<NotConnectedState>();
}

void ConnectingState::handleTimeout()
{
    context.timer.stopTimer();

    context.setState<NotConnectedState>();
}

void ConnectingState::handleDisconnect()
{
    context.timer.stopTimer();
    context.setState<NotConnectedState>();
}

} // namespace ue
