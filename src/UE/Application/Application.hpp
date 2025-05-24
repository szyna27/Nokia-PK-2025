#pragma once

#include "States/NotConnectedState.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Messages/MessageId.hpp"
#include "IEventsHandler.hpp"
#include "SMS/SMSDB.hpp"
#include "Context.hpp"
#include "States/ViewSMSListState.hpp"


namespace ue
{

using common::PhoneNumber;
using common::ILogger;

class Application : public IEventsHandler
{
public:
    Application(PhoneNumber phoneNumber,
                ILogger& iLogger,
                IBtsPort& bts,
                IUserPort& user,
                ITimerPort& timer,
                SMSDB& smsDB);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnect() override;

    // Call events
    void handleCallRequest(common::PhoneNumber from) override;
    void handleCallTalk(const std::string message) override;
    void handleCallAccept(common::PhoneNumber from) override;
    void handleCallDropped(common::PhoneNumber from) override;

    // SMS events
    void handleSMS(common::PhoneNumber from, const std::string& message) override;
    
    void handleUIAction(std::optional<std::size_t> selectedIndex) override;
    void handleUIBack() override;

private:
    Context context;
    common::PrefixedLogger logger;

};

}
