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

    void setAcceptCallback(IUeGui::Callback callback) override;
    void setRejectCallback(IUeGui::Callback callback) override;
    void setHomeCallback(IUeGui::Callback callback) override;
    void setItemSelectedCallback(IUeGui::Callback callback) override;

private:
    common::PrefixedLogger logger;
    IUeGui::Callback callback;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
};

}
