#pragma once
#include "Messages/PhoneNumber.hpp"
#include "IUeGui.hpp"
#include <optional>
#include "SMS/SMSDB.hpp"
namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
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

    // SMS views
    virtual void showViewSms(SMS sms) = 0;
    virtual void showNewSms(bool present) = 0;
    virtual void showSmsList(const std::vector<ue::SMS>&) = 0;

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

    // Calling UI methods
    // virtual void showCallRequest(common::PhoneNumber) = 0;
    // virtual void showCallAccept(common::PhoneNumber) = 0;
    // virtual void showCallDropped(common::PhoneNumber) = 0;
    // virtual void showCallTalk(common::PhoneNumber) = 0;
};

}
