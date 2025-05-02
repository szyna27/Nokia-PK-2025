#include "Ports/UserPort.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ComposeSmsState.hpp"
#include "ViewSmsState.hpp"
#include "DialState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
    context.user.setItemSelectedCallback([this, &menu] { selectMode(); });
}

void ConnectedState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::selectMode(IUeGui::IListViewMode &menu)
{
    auto indexPair = menu.getCurrentItemIndex();
    auto exists = indexPair.first;
    auto mode = indexPair.second;
    if (!exists)
    {
        logger.logError("No item selected");
        return;
    }

    switch (mode)
    {
        case COMPOSE_SMS:
            context.setState<ComposeSmsState>();
            break;
        case VIEW_SMS:
            context.setState<ViewSmsState>();    
            break;
        case DIAL:
            context.setState<DialState>();
            break;
        default:
            logger.logError("Unknown mode: ", mode);
            break;
    }
}

}
