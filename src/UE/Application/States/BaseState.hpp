#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnect() override;
    void handleCallTalk(const std::string message) override;
    void handleCallAccept(common::PhoneNumber) override;
    void handleCallDropped(common::PhoneNumber) override;
    void handleSMS(common::PhoneNumber from, const std::string& message) override;
    void handleAccept();
    void handleCallRequest(common::PhoneNumber from) override;

    void handleUIAction(std::optional<std::size_t> selectedIndex) override;
    void handleUIBack() override;

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
