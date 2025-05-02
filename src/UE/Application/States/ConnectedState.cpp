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
    auto &menu = context.user.getMenuObject();
    context.user.setItemSelectedCallback([this, &menu] { selectMode(menu); });
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
            logger.logInfo("Compose SMS selected");
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
