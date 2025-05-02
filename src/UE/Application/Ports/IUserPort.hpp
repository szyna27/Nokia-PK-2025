#pragma once
#include "Messages/PhoneNumber.hpp"

#include "IUeGui.hpp"

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
    virtual void showComposeSms() = 0;
    virtual void showViewSms() = 0;
    virtual void showDial() = 0;
    virtual void showTalking() = 0;

    virtual void setAcceptCallback(IUeGui::Callback) = 0;
    virtual void setRejectCallback(IUeGui::Callback) = 0;
    virtual void setHomeCallback(IUeGui::Callback) = 0;
    virtual void setItemSelectedCallback(IUeGui::Callback) = 0;
    
    virtual IUeGui::IListViewMode& getMenuObject() = 0;
    virtual void showSms(common::PhoneNumber from, std::string message) = 0;
};

}
