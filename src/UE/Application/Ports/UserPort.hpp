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

    // SMS views
    void showViewSms(SMS sms) override;
    void showNewSms(bool present) override;
    void showSmsList(const std::vector<SMS>& smsList) override;

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

    // Calling UI methods
    // TODO: Consider Implement these methods
    // void showCallRequest(common::PhoneNumber) override;
    // void showCallAccept(common::PhoneNumber) override;
    // void showCallDropped(common::PhoneNumber) override;
    // void showCallTalk(common::PhoneNumber) override;

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

    // Callbacks for UI events
    void handleDialClicked();
    void handleAcceptDialClicked();
    void handleRejectDialClicked();
    void handleAcceptCallClicked();
    void handleRejectCallClicked();
};

}
