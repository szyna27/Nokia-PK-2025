#pragma once

#include "Messages/PhoneNumber.hpp"
#include "IUeGui.hpp"
#include <optional>

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleUIAction(std::optional<std::size_t> selectedIndex) = 0;
    virtual void handleUIBack() = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showMainMenu() = 0;
    virtual void showComposeSms() = 0;
    virtual void showViewSms() = 0;
    virtual void showNewSms(bool present) = 0;
    virtual void showDial() = 0;
    virtual void showTalking() = 0;

    virtual void setAcceptCallback(IUeGui::Callback) = 0;
    virtual void setRejectCallback(IUeGui::Callback) = 0;
    virtual void setHomeCallback(IUeGui::Callback) = 0;
    virtual void setItemSelectedCallback(IUeGui::Callback) = 0;
    
    virtual IUeGui::IListViewMode& getListViewMode() = 0;
    
    virtual IUeGui::ISmsComposeMode& getSmsComposeMode() = 0;
    virtual IUeGui::ITextMode& getViewSmsMode() = 0;
    virtual IUeGui::IDialMode& getDialMode() = 0;
    virtual IUeGui::ICallMode& getCallMode() = 0;
};

}
