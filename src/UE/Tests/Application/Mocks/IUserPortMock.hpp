#pragma once

#include <gmock/gmock.h>
#include "Ports/IUserPort.hpp"

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

    MOCK_METHOD(void, handleUIEnding, (), (override));
    MOCK_METHOD(void, handleUICalling, (std::optional<std::size_t> selectedIndex), (override));
    MOCK_METHOD(void, handleCallAccepted, (common::PhoneNumber recipient), (override));
    MOCK_METHOD(void, handleUnknownRecipient, (common::PhoneNumber recipient), (override));
    MOCK_METHOD(void, handleCallDropped, (), (override));
};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showAlert, (const std::string &header, const std::string &msg), (override));

};

}
