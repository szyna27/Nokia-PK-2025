#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"

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
    void showMainMenu() override;
    void showComposeSms() override;
    void showViewSms() override;
    void showNewSms(bool present) override;
    void showDial() override;
    void showTalking() override;
    
    void setAcceptCallback(IUeGui::Callback callback) override;
    void setRejectCallback(IUeGui::Callback callback) override;
    void setHomeCallback(IUeGui::Callback callback) override;
    void setItemSelectedCallback(IUeGui::Callback callback) override;
    
    IUeGui::IListViewMode& getListViewMode() override;
    IUeGui::ISmsComposeMode& getSmsComposeMode() override;
    IUeGui::ITextMode& getViewSmsMode() override;
    IUeGui::IDialMode& getDialMode() override;
    IUeGui::ICallMode& getCallMode() override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
    IUeGui::IListViewMode* listViewMode = nullptr;
    IUeGui::ISmsComposeMode* smsComposeMode = nullptr;
    IUeGui::ITextMode* viewSmsMode = nullptr;
    IUeGui::IDialMode* dialMode = nullptr;
    IUeGui::ICallMode* callMode = nullptr;
};

}
