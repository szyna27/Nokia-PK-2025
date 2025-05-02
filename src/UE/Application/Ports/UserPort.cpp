#include "UserPort.hpp"
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
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menuObject = &menu;
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    menu.addSelectionListItem("Dial", "");
}

void UserPort::showComposeSms()
{
    IUeGui::ISmsComposeMode& smsComposeMode = gui.setSmsComposeMode();
}

void UserPort::showViewSms()
{
    IUeGui::ITextMode& viewSmsMode = gui.setViewTextMode();
}

void UserPort::showDial()
{
    IUeGui::IDialMode& dialMode = gui.setDialMode();
}

void UserPort::showTalking()
{
    IUeGui::ICallMode& callMode = gui.setCallMode();
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

IUeGui::IListViewMode &UserPort::getMenuObject()
{
    if (!menuObject)
    {
        menuObject = &gui.setListViewMode();
        logger.logError("Menu object is not initialized");
    }
    return *menuObject;
}

void UserPort::showSms(common::PhoneNumber from, std::string message)
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("From: " + to_string(from), message);
}
}
