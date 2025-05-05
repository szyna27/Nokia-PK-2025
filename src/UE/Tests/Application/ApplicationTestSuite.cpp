#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;
using namespace std::chrono_literals;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{1024};
    NiceMock<common::ILoggerMock> loggerMock;

    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    SMSDB smsDB = SMSDB();

    Application objectUnderTest{PHONE_NUMBER,
        loggerMock,
        btsPortMock,
        userPortMock,
        timerPortMock,
        smsDB};  
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{
    void shallHandleSibMessage()
    {
        EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
        EXPECT_CALL(timerPortMock, startTimer(500ms));
        EXPECT_CALL(userPortMock, showConnecting());

        objectUnderTest.handleSib(BTS_ID);   
    }
};

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    ApplicationConnectingTestSuite()
    {
        shallHandleSibMessage();
        EXPECT_CALL(timerPortMock, stopTimer());
    }

    void shallHandleAttachAccept()
    {
        EXPECT_CALL(userPortMock, showConnected());
        EXPECT_CALL(userPortMock, showMainMenu());
        EXPECT_CALL(userPortMock, getListViewMode()).WillOnce(ReturnRef(listViewModeMock));
        EXPECT_CALL(userPortMock, setItemSelectedCallback(_));
        EXPECT_CALL(userPortMock, setHomeCallback(_));
        EXPECT_CALL(userPortMock, setAcceptCallback(_));
        EXPECT_CALL(userPortMock, setRejectCallback(_));

        objectUnderTest.handleAttachAccept();
    }

    void shallHandleAttachReject()
    {
        EXPECT_CALL(userPortMock, showNotConnected());

        objectUnderTest.handleAttachReject();
    }

    void shallHandleTimeout()
    {
        EXPECT_CALL(userPortMock, showNotConnected());

        objectUnderTest.handleTimeout();
    }

    void shallHandleDisconnect()
    {
        EXPECT_CALL(userPortMock, showNotConnected());

        objectUnderTest.handleDisconnect();
    }

    void shallHandleReconnect()
    {
        shallHandleDisconnect();
        shallHandleSibMessage();
        EXPECT_CALL(timerPortMock, stopTimer());
        shallHandleAttachAccept();
    }
};

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedTestSuite()
    {
        shallHandleAttachAccept();
    }
};


TEST_F(ApplicationNotConnectedTestSuite, shallHandleSibMessage)
{
    shallHandleSibMessage();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleAttachAccept)
{
    shallHandleAttachAccept();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleAttachReject)
{
    shallHandleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleTimeout)
{
    shallHandleTimeout();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleDisconnectFromConnecting)
{
    shallHandleDisconnect();
}

TEST_F(ApplicationConnectingTestSuite, shallHandleReconnectFromConnecting)
{
    shallHandleReconnect();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleDisconnectFromConnected)
{
    shallHandleDisconnect();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReconnectFromConnected)
{
    shallHandleReconnect();
}

}