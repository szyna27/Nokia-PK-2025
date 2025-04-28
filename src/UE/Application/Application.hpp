#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"

namespace ue
{

    using common::ILogger;
    using common::PhoneNumber;

    class Application : public IEventsHandler
    {
    public:
        Application(PhoneNumber phoneNumber,
                    ILogger &iLogger,
                    IBtsPort &bts,
                    IUserPort &user,
                    ITimerPort &timer);
        ~Application();

        // ITimerEventsHandler interface
        void handleTimeout() override;

        // IBtsEventsHandler interface
        void handleSib(common::BtsId btsId) override;
        void handleAttachAccept() override;
        void handleAttachReject() override;
        void handleDisconnect() override;

        // IUserEventHandler iterface
        void handleUIEnding() override;
        void handleUICalling(std::optional<std::size_t> selectedIndex) override;
        void handleCallAccepted(common::PhoneNumber recipient) override;
        void handleUnknownRecipient(common::PhoneNumber recipient) override;
        void handleCallDropped() override;

    private:
        Context context;
        common::PrefixedLogger logger;
    };

}
