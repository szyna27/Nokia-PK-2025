#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "UeGui/IListViewMode.hpp"

namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();
    
    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showComposeSms() override;
    void showViewSms() override;
    void showDial() override;
    void showTalking() override;
    
    void setAcceptCallback(IUeGui::Callback callback) override;
    void setRejectCallback(IUeGui::Callback callback) override;
    void setHomeCallback(IUeGui::Callback callback) override;
    void setItemSelectedCallback(IUeGui::Callback callback) override;
    
    IUeGui::IListViewMode& getMenuObject() override;
    IUeGui::ISmsComposeMode& getSmsComposeMode() override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
    IUeGui::IListViewMode* menuObject = nullptr;
    IUeGui::ISmsComposeMode* smsComposeMode = nullptr;
};

}
