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

};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showMainMenu, (), (final));
    MOCK_METHOD(void, showComposeSms, (), (final));
    MOCK_METHOD(void, showViewSms, (), (final));
    MOCK_METHOD(void, showDial, (), (final));
    MOCK_METHOD(void, showTalking, (), (final));

    MOCK_METHOD(void, setAcceptCallback, (IUeGui::Callback), (final));
    MOCK_METHOD(void, setRejectCallback, (IUeGui::Callback), (final));
    MOCK_METHOD(void, setHomeCallback, (IUeGui::Callback), (final));
    MOCK_METHOD(void, setItemSelectedCallback, (IUeGui::Callback), (final));
    
    MOCK_METHOD(IUeGui::IListViewMode&, getListViewMode, (), (final));
    MOCK_METHOD(IUeGui::ISmsComposeMode&, getSmsComposeMode, (), (final));
    MOCK_METHOD(IUeGui::ITextMode&, getViewSmsMode, (), (final));
    MOCK_METHOD(IUeGui::IDialMode&, getDialMode, (), (final));
    MOCK_METHOD(IUeGui::ICallMode&, getCallMode, (), (final));
};

}
