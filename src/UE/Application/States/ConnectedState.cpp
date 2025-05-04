#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
    changeMode(MAIN_MENU);
}

void ConnectedState::handleDisconnect()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::selectMode(IUeGui::IListViewMode &menu)
{
    auto indexPair = menu.getCurrentItemIndex();
    auto exists = indexPair.first;
    unsigned int mode = indexPair.second;
    if (!exists)
    {
        logger.logError("No item selected");
        return;
    }

    changeMode(mode);
}

void ConnectedState::changeMode(unsigned int mode)
{
    switch (mode)
    {
        case COMPOSE_SMS:
            logger.logInfo("Changing mode to compose SMS");
            context.user.showComposeSms();

            context.user.setAcceptCallback([this] { sendSMS(); });

            break;

        case VIEW_SMS:
            logger.logInfo("Changing mode to view SMS");
            context.user.showViewSms();

            break;

        case DIAL:
            logger.logInfo("Changing mode to dial");
            context.user.showDial();

            break;

        case MAIN_MENU:
            {   // Zmienna menu musi być wewnątrz bloku, stąd klamry
                logger.logInfo("Changing mode to main menu");
                context.user.showMainMenu();

                auto &menu = context.user.getListViewMode();
                context.user.setItemSelectedCallback([this, &menu] { selectMode(menu); });
                context.user.setHomeCallback([this] { changeMode(MAIN_MENU); });
                context.user.setAcceptCallback([this] { nullptr; });
                context.user.setRejectCallback([this] { nullptr; });

                break;
            }

        default:
            logger.logError("Unknown mode: ", mode);
            break;
    }
}

void ConnectedState::sendSMS()
{
    logger.logInfo("Sending SMS");
    auto &smsComposeMenu = context.user.getSmsComposeMode();
    PhoneNumber phoneNumber = smsComposeMenu.getPhoneNumber();
    std::string smsText = smsComposeMenu.getSmsText();
    context.bts.sendSMS(phoneNumber, smsText);
}

void ConnectedState::handleSMS(PhoneNumber from, const std::string &message)
{
    logger.logInfo("Received SMS from: ", from, " message: ", message);
    context.smsDB.addSMS(from, message);
    context.user.showNewSms(true);
}
}
