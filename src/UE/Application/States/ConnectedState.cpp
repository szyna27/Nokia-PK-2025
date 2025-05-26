#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"
#include "ViewSMSListState.hpp"


using namespace std::chrono_literals;

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
            logger.logInfo("Compose SMS mode");

            context.user.setAcceptCallback([this] { sendSMS(); });
            context.user.setRejectCallback([this] { changeMode(MAIN_MENU); });

            break;

        case VIEW_SMS:
            logger.logInfo("Changing mode to view SMS");
            
            context.setState<ViewSMSListState>();
            break;

        case DIAL:
            context.user.getCallMode().clearIncomingText();
            logger.logInfo("Changing mode to dial");
            context.user.showDial();
            logger.logInfo("Dial mode");
            
            context.user.setAcceptCallback([this] { sendCallRequest(); });
            context.user.setRejectCallback([this] { changeMode(MAIN_MENU); });
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

void ConnectedState::sendCallRequest()
{
    auto &dialMenu = context.user.getDialMode();
    context.user.getCallMode().clearIncomingText();
   
    PhoneNumber toNumber = dialMenu.getPhoneNumber();
    
    PhoneNumber fromNumber = context.bts.getPhoneNumber();
    
    if(toNumber == fromNumber)
    {
        logger.logError("Cannot call yourself");
        context.setState<ConnectedState>();
        return;
    }

    isCalling = true;
    logger.logInfo("Sending call request");
    context.user.setRejectCallback([this, toNumber] { sendCallDrop(toNumber); });
    context.bts.sendCallRequest(toNumber);
    context.user.setHomeCallback([this] { nullptr; });
    context.timer.startTimer(60000ms);
}

void ConnectedState::handleCallRequest(PhoneNumber from)
{
    if(isCalling){
        logger.logInfo("Sending call drop");
        context.bts.sendCallDrop(from);

        logger.logError("Received call request from: ", from, " while already in a call");
        return;
    }

    isCalling = true;
    logger.logInfo("Received call request from: ", from);
    changeMode(DIAL);
    context.user.getCallMode().clearIncomingText();
    context.user.getCallMode().appendIncomingText("Incoming call from: " + std::to_string(from.value));
    context.user.setAcceptCallback([this, from] { sendCallAccept(from); });
    context.user.setRejectCallback([this, from] { sendCallDrop(from); });
    context.user.setHomeCallback([this] { nullptr; });
}

void ConnectedState::sendCallAccept(PhoneNumber from)
{
    logger.logInfo("Sending call accept");
    context.bts.sendCallAccept(from);
    context.setState<TalkingState>(from);
    context.timer.startTimer(std::chrono::milliseconds(30000));
}

void ConnectedState::sendCallDrop(PhoneNumber from)
{
    context.timer.stopTimer();
    logger.logInfo("Sending call drop");
    context.bts.sendCallDrop(from);

    isCalling = false;
    context.setState<ConnectedState>();
}

void ConnectedState::handleCallDropped(PhoneNumber from)
{
    context.timer.stopTimer();
    logger.logInfo("Received call dropped from: ", from);
    isCalling = false;
    changeMode(MAIN_MENU);
}

void ConnectedState::handleCallAccept(PhoneNumber from)
{
    context.timer.stopTimer();
    context.user.getCallMode().clearIncomingText();
    logger.logInfo("Received call accept from: ", from);
    context.setState<TalkingState>(from);
    context.timer.startTimer(std::chrono::milliseconds(30000));
}

void ConnectedState::handleTimeout()
{
    logger.logInfo("Received timeout");
    dropCall();
}

void ConnectedState::dropCall(){
    auto &dialMenu = context.user.getDialMode();
    PhoneNumber phoneNumber = dialMenu.getPhoneNumber();
    logger.logInfo("Dropping outgoing call request to: ", phoneNumber);
    context.bts.sendCallDrop(phoneNumber);
    isCalling = false;
    changeMode(MAIN_MENU);
}

void ConnectedState::handleUIAction(std::optional<std::size_t> selectedIndex)
{
    if (!selectedIndex.has_value())
    {
        logger.logError("Unexpected: handleUiAction: no selected index");
        return;
    }

    auto index = selectedIndex.value();

    if (index == 1) 
    {
        context.setState<ViewSMSListState>();
        return;
    }

    logger.logError("Unexpected: handleUiAction");

} 

void ConnectedState::handleUIBack()
{
    logger.logInfo("Unexpected: handleUiBack");
}

}// namespace ue