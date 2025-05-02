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
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    menu.addSelectionListItem("Dial", "");
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

void UserPort::switchMode(IUeGui::IListViewMode &menu)
{
    auto indexPair = menu.getCurrentItemIndex();

    if (indexPair.first)
    {
        currentMode = indexPair.second;
    }
    else
    {
        currentMode = NONE;
    }
}

int UserPort::getCurrentMode()
{
    return currentMode;
}

}