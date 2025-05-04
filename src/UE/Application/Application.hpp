#pragma once

#include "States/NotConnectedState.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Messages/MessageId.hpp"
#include "IEventsHandler.hpp"
#include "SMS/SMSDB.hpp"
#include "Context.hpp"

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
    void handleCallMessage(common::MessageId msgId) override;
    void handleCallAccept(common::MessageId msgId) override;
    void handleCallDropped(common::MessageId msgId) override;
    void handleSMS(common::PhoneNumber from, const std::string& message) override;

private:
    Context context;
    common::PrefixedLogger logger;

};

}
