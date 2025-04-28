#pragma once
#include <string>
#include <optional>
#include "Messages/PhoneNumber.hpp"

namespace ue
{

    class IUserEventsHandler
    {
    public:
        virtual ~IUserEventsHandler() = default;

        virtual void handleCallDropped() = 0;
        virtual void handleUIEnding() = 0;
        virtual void handleUICalling(std::optional<std::size_t> selectedIndex) = 0;
        virtual void handleCallAccepted(common::PhoneNumber recipient) = 0;
        virtual void handleUnknownRecipient(common::PhoneNumber recipient) = 0;
    };

    class IUserPort
    {
    public:
        virtual ~IUserPort() = default;

        virtual void showNotConnected() = 0;
        virtual void showConnecting() = 0;
        virtual void showConnected() = 0;
        virtual void showAlert(const std::string &header, const std::string &msg) = 0;
    };

}
