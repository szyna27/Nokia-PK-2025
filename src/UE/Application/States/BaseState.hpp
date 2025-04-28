#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue
{

    class BaseState : public IEventsHandler
    {
    public:
        BaseState(Context &context, const std::string &name);
        ~BaseState() override;

        // ITimerEventsHandler interface
        void handleTimeout() override;

        // IBtsEventsHandler interface
        void handleSib(common::BtsId btsId) override;
        void handleAttachAccept() override;
        void handleAttachReject() override;
        void handleDisconnect() override;

        void handleUIEnding() override;
        void handleUICalling(std::optional<std::size_t> selectedIndex) override;
        void handleCallAccepted(common::PhoneNumber recipient) override;
        void handleUnknownRecipient(common::PhoneNumber recipient) override;
        void handleCallDropped() override;

    protected:
        Context &context;
        common::PrefixedLogger logger;
    };

}
