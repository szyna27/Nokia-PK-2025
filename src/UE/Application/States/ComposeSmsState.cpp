#include "ComposeSmsState.hpp"
#include "ConnectedState.hpp"
#include "UeGui/ISmsComposeMode.hpp"

namespace ue
{

ComposeSmsState::ComposeSmsState(Context &context)
    : BaseState(context, "ComposeSmsState")
{
    logger.logInfo("Entering ComposeSmsState");
    context.user.showComposeSms();
    context.user.setHomeCallback([this] { returnToConnectedState(); });
    context.user.setAcceptCallback([this] {
        sendSMS();
    });
}

void ComposeSmsState::returnToConnectedState()
{
    logger.logInfo("Returning to ConnectedState");
    context.setState<ConnectedState>();
}

void ComposeSmsState::sendSMS()
{
    logger.logInfo("Sending SMS");
    auto &smsComposeMenu = context.user.getSmsComposeMode();
    PhoneNumber phoneNumber = smsComposeMenu.getPhoneNumber();
    std::string smsText = smsComposeMenu.getSmsText();
    context.bts.sendSMS(phoneNumber, smsText);
}
}
