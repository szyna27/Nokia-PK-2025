#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include <functional>

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected()
{
    gui.showConnected();
}

void UserPort::showMainMenu()
{
    IUeGui::IListViewMode& listViewMode = gui.setListViewMode();
    this->listViewMode = &listViewMode;
    listViewMode.clearSelectionList();
    listViewMode.addSelectionListItem("Compose SMS", "");
    listViewMode.addSelectionListItem("View SMS", "");
    listViewMode.addSelectionListItem("Dial", "");
}

void UserPort::showComposeSms()
{
    IUeGui::ISmsComposeMode& smsComposeMode = gui.setSmsComposeMode();
    this->smsComposeMode = &smsComposeMode;
}

void UserPort::showViewSms()
{
    IUeGui::ITextMode& viewSmsMode = gui.setViewTextMode();
    this->viewSmsMode = &viewSmsMode;
}

void UserPort::showDial()
{
    IUeGui::IDialMode& dialMode = gui.setDialMode();
    this->dialMode = &dialMode;
}

void UserPort::showTalking()
{
    IUeGui::ICallMode& callMode = gui.setCallMode();
    this->callMode = &callMode;
}

void UserPort::setAcceptCallback(IUeGui::Callback callback)
{
    gui.setAcceptCallback(callback);
}

void UserPort::setRejectCallback(IUeGui::Callback callback)
{
    gui.setRejectCallback(callback);
}

void UserPort::setHomeCallback(IUeGui::Callback callback)
{
    gui.setHomeCallback(callback);
}

void UserPort::setItemSelectedCallback(IUeGui::Callback callback)
{
    gui.setItemSelectedCallback(callback);
}

IUeGui::IListViewMode &UserPort::getListViewMode()
{
    if (!listViewMode)
    {
        listViewMode = &gui.setListViewMode();
        logger.logError("Menu object is not initialized");
    }
    return *listViewMode;
}

IUeGui::ISmsComposeMode &UserPort::getSmsComposeMode()
{
    if(!smsComposeMode)
    {
        smsComposeMode = &gui.setSmsComposeMode();
        logger.logError("SMS compose mode is not initialized");
    }
    return *smsComposeMode;
}

IUeGui::ITextMode &UserPort::getViewSmsMode()
{
    if (!viewSmsMode)
    {
        viewSmsMode = &gui.setViewTextMode();
        logger.logError("View SMS mode is not initialized");
    }
    return *viewSmsMode;
}

IUeGui::IDialMode &UserPort::getDialMode()
{
    if (!dialMode)
    {
        dialMode = &gui.setDialMode();
        logger.logError("Dial mode is not initialized");
    }
    return *dialMode;
}

IUeGui::ICallMode &UserPort::getCallMode()
{
    if (!callMode)
    {
        callMode = &gui.setCallMode();
        logger.logError("Call mode is not initialized");
    }
    return *callMode;
}

// void UserPort::showComposeSMS()
// {
//     printf("UserPort::showComposeSMS\n");
//     IUeGui::ISmsComposeMode& smsComposeMode = gui.setSmsComposeMode();
//     gui.setAcceptCallback([this, &smsComposeMode] {
//         common::PhoneNumber to = smsComposeMode.getPhoneNumber();
//         std::string message = smsComposeMode.getSmsText();
//         gui.sendSms(to, message);
//     });
//     gui.setRejectCallback([this] { });
// }
}
